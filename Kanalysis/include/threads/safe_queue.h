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

#pragma once

#include "include/config.h"

namespace kanalysis::threads
{
	template<typename T>
	class SafeQueue
	{
	public:
		bool empty();
		std::size_t size();
		void reserve(std::size_t size);
		void enqueue(T& t);
		bool dequeue(T& t);
	private:
		std::mutex m_mutex;
		std::vector<T> m_queue;
	};
} // namespace kanalysis::threads

// https://github.com/mtrebi/thread-pool/blob/master/include/SafeQueue.h
namespace kanalysis::threads
{
	template<typename T>
	bool SafeQueue<T>::empty()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	template<typename T>
	std::size_t SafeQueue<T>::size()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

	template<typename T>
	void SafeQueue<T>::reserve(std::size_t size)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.reserve(size);
	}

	template<typename T>
	void SafeQueue<T>::enqueue(T& t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push_back(t);
	}

	template<typename T>
	bool SafeQueue<T>::dequeue(T& t)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_queue.empty()) return false;
		t = std::move(m_queue.front());
		m_queue.erase(m_queue.begin());
		return true;
	}
} // namespace kanalysis::threads