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

// https://github.com/mtrebi/thread-pool/blob/master/include/SafeQueue.h

#pragma once

#include "include/config.h"

namespace kanalysis::threads
{
	///
	/// \brief A thread-safe implementation of std::queue.
	///
	template<typename T>
	class SafeQueue
	{
	public:
		bool empty();
		std::size_t size();
		void reserve(std::size_t n);
		void enqueue(T& t);
		bool dequeue(T& t);
	private:
		std::mutex m_mutex;
		std::vector<T> m_queue;
	};
} // namespace kanalysis::threads

namespace kanalysis::threads
{
	///
	/// \brief Checks whether the underlying container is empty.
	///
	/// \return \a true if the container is empty, else \a false .
	///
	template<typename T>
	bool SafeQueue<T>::empty()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	///
	/// \brief Returns the number of elements of the underlying container.
	///
	/// \return The number of elements in the container.
	///
	template<typename T>
	std::size_t SafeQueue<T>::size()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

	///
	/// \brief Increase the allocated capacity or number of elements that the underlying container can hold.
	///
	/// \param n An integer representing the number of elements to be reserved by.
	///
	template<typename T>
	void SafeQueue<T>::reserve(std::size_t n)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.reserve(n);
	}

	///
	/// \brief Appends the object \a t into the back of the queue.
	///
	/// \param t An object of type \a T .
	///
	template<typename T>
	void SafeQueue<T>::enqueue(T& t) // To-do: T&&
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push_back(t); // To-do: emplace_back
	}

	///
	/// \brief std::move the queued object at the front of the queue to \a t .
	///
	/// \param t An empty object of type \a T . This is the \a out parameter.
	/// \return \a true if \a t now holds the dequeued object, else \a false .
	///
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
