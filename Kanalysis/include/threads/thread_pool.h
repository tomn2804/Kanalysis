// MIT License
//
// Copyright (c) 2016 Mariano Trebino
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// https://github.com/mtrebi/thread-pool/blob/master/include/ThreadPool.h

#pragma once

#include "include/config.h"

#include "include/threads/safe_queue.h"
#include "include/utils/progress.h"

namespace kanalysis::threads
{
	class ThreadPool
	{
	public:
		ThreadPool(int threads);

		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;

		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		void start();
		void shutdown();
		void reserve(std::size_t size);

		template<typename F, typename...Args>
		auto submit(F&& f, Args&&... args);

		std::size_t size() const;
	protected:
		std::mutex m_conditional_mutex;
		std::condition_variable m_conditional_lock;
	private:
		class ThreadWorker
		{
		public:
			ThreadWorker(ThreadPool& pool, int id);
			void operator()();
		private:
			int m_id;
			ThreadPool& m_pool;
		};

		bool m_shutdown;
		std::vector<std::thread> m_threads;
		SafeQueue<std::function<void(int)>> m_queue;

		utils::Progress m_status;
	};
} // namespace kanalysis::threads

namespace kanalysis::threads
{
	KANALYSIS_INLINE ThreadPool::ThreadPool(int threads)
		: m_shutdown(true)
		, m_threads(threads)
	{}

	KANALYSIS_INLINE void ThreadPool::start()
	{
		m_status.start();
		m_shutdown = false;
		for (int i = 0; i < m_threads.size(); ++i)
		{
			m_threads[i] = std::thread(ThreadWorker(*this, i));
		}
	}

	KANALYSIS_INLINE void ThreadPool::shutdown()
	{
		m_shutdown = true;
		m_conditional_lock.notify_all();
		for (auto& thread : m_threads)
		{
			if (thread.joinable()) thread.join();
		}
		m_status.complete();
	}

	KANALYSIS_INLINE void ThreadPool::reserve(std::size_t size)
	{
		m_queue.reserve(size);
		m_status = utils::Progress(size);
	}

	template<typename F, typename...Args>
	auto ThreadPool::submit(F&& f, Args&&... args)
	{
		auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		auto task_ptr = std::make_shared<std::packaged_task<decltype(func(std::declval<int>()))(int)>>(func);
		std::function<void(int)> wrapper_func = [task_ptr](int worker_id) { (*task_ptr)(worker_id); };

		m_queue.enqueue(wrapper_func);
		m_conditional_lock.notify_one();

		return task_ptr->get_future();
	}

	KANALYSIS_INLINE std::size_t ThreadPool::size() const
	{
		return m_threads.size();
	}

	KANALYSIS_INLINE ThreadPool::ThreadWorker::ThreadWorker(ThreadPool& pool, int id)
		: m_id(id)
		, m_pool(pool)
	{}

	KANALYSIS_INLINE void ThreadPool::ThreadWorker::operator()()
	{
		std::function<void(int)> func;
		bool dequeued;
		while (!m_pool.m_shutdown)
		{
			{
				std::unique_lock<std::mutex> lock(m_pool.m_conditional_mutex);
				if (m_pool.m_queue.empty()) m_pool.m_conditional_lock.wait(lock);
				dequeued = m_pool.m_queue.dequeue(func);
			}
			if (dequeued)
			{
				func(m_id);
				{
					std::unique_lock<std::mutex> lock(m_pool.m_conditional_mutex);
					m_pool.m_status.increment();
				}
			}
		}
	}
} // namespace kanalysis::threads
