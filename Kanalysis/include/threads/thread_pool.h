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

// https://github.com/mtrebi/thread-pool/blob/master/ThreadPool.h

#pragma once

#include "config.h"

#include "threads/safe_queue.h"
#include "utils/progress.h"

namespace kanalysis::threads
{
	///
	/// \brief A thread pool that manages and assigns tasks to worker threads to be executed asynchronously.
	///
	class ThreadPool
	{
	public:
		ThreadPool(int threads);

		// Suppress copy and move constructors.
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;

		// Suppress copy and move assignment operators.
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		void start();
		void shutdown();

		std::size_t size() const;
		void reserve(std::size_t n);

		template<typename F, typename... Args>
		auto submit(F&& f, Args&&... args);
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
			ThreadPool& m_pool;
			int m_id;
		};

		std::vector<std::thread> m_threads;
		SafeQueue<std::function<void(int)>> m_queue;

		utils::Progress m_status;
		bool m_shutdown = true;
	};
} // namespace kanalysis::threads

namespace kanalysis::threads
{
	///
	/// \brief A constructor.
	///
	/// \details The pool is not active until \a start() is called.
	///
	/// \param threads Number of threads to be allocated.
	///
	KANALYSIS_INLINE ThreadPool::ThreadPool(int threads)
		: m_threads(threads)
	{}

	///
	/// \brief Starts the thread pool.
	///
	/// \details if \a reserve() is called beforehand then the console will display the progress percentage.
	///
	KANALYSIS_INLINE void ThreadPool::start()
	{
		if (m_status.n() != 0) m_status.start();
		m_shutdown = false;
		for (std::size_t i = 0; i < m_threads.size(); ++i)
		{
			m_threads[i] = std::thread(ThreadWorker(*this, i));
		}
	}

	///
	/// \brief Shutdown the thread pool.
	///
	KANALYSIS_INLINE void ThreadPool::shutdown()
	{
		m_shutdown = true;
		m_conditional_lock.notify_all();
		for (auto& thread : m_threads)
		{
			if (thread.joinable()) thread.join();
		}
		if (m_status.n() != 0) m_status.complete();
	}

	///
	/// \return The number of elements in the queue.
	///
	KANALYSIS_INLINE std::size_t ThreadPool::size() const
	{
		return m_threads.size();
	}

	///
	/// \brief Increase the allocated capacity or number of elements that the queue can hold.
	///
	/// \details Calling this function will enables (not yet display) the progress percentage in console.
	/// To display the progress percentage, \a start() or \a shutdown() must be called.
	///
	/// \param n An integer representing the number of elements to be reserved by.
	///
	KANALYSIS_INLINE void ThreadPool::reserve(std::size_t n)
	{
		m_queue.reserve(n);
		m_status = utils::Progress(n);
	}

	///
	/// \brief Submit a task into the pool.
	/// And an idling worker thread will automatically retrieve the task and execute it asynchronously.
	///
	/// \details The task can be a function or a functor.
	///
	/// \details Additionally, the task function can know which thread is currently calling on it by accepting an id number of the worker thread as argument.
	/// To do this, \a std::placeholders::_1 must be passed into \a args .
	/// And the task function signature must accept an \a int in the parameter.
	/// The order of \a std::placeholders::_1 in \a args and parameter \a int in the task function must correspond with each other.
	///
	/// \param f A function or a functor name.
	/// \param args Parameter arguments for the function or functor \a f .
	/// \return A \a std::future object of the task function.
	///
	template<typename F, typename... Args>
	auto ThreadPool::submit(F&& f, Args&&... args)
	{
		auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		auto task_ptr = std::make_shared<std::packaged_task<decltype(func(std::declval<int>()))(int)>>(func);
		std::function<void(int)> wrapper_func = [task_ptr](int worker_id)
		{
			(*task_ptr)(worker_id);
		};

		m_queue.enqueue(wrapper_func);
		m_conditional_lock.notify_one();

		return task_ptr->get_future();
	}

	///
	/// \brief A constructor.
	///
	/// \param pool The parent thread pool.
	/// \param id An interger assigning a worker thread an id. This \a id can be optionally passed into the task function/functor as parameter argument.
	///
	KANALYSIS_INLINE ThreadPool::ThreadWorker::ThreadWorker(ThreadPool& pool, int id)
		: m_pool(pool)
		, m_id(id)
	{}

	///
	/// \brief A call operator for worker thread.
	///
	/// \details If the pool is not empty, the worker thread will keep retrieving and executing tasks until the pool is empty. Once empty, the worker thread will go into an idle state.
	/// To wake up an idling worker thread, the pool must call \a m_conditional_lock.notify_one() or \a m_conditional_lock.notify_all() .
	///
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
