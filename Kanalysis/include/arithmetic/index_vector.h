#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic
{
	class IndexVector
	{
	public:
		using Iterator = Index*;
		using ReverseIterator = std::reverse_iterator<Index*>;

		IndexVector();
		IndexVector(Index n, Index r);
		IndexVector(Index n);

		IndexVector(const IndexVector& other);
		IndexVector(IndexVector&& other) noexcept;

		IndexVector& operator=(const IndexVector& rhs);
		IndexVector& operator=(IndexVector&& rhs) noexcept;

		void set_middle(Index r);
		void print() const;

		Index n() const;
		Index r() const;

		std::vector<Index> indicies() const;

		Iterator begin() const;
		Iterator middle() const;
		Iterator end() const;

		ReverseIterator rbegin() const;
		ReverseIterator rmiddle() const;
		ReverseIterator rend() const;
	private:
		void update_iterator();

		Index m_n;
		Index m_r;

		std::vector<Index> m_indicies;

		Iterator m_first;
		Iterator m_middle; // Position = m_first + m_r
		Iterator m_last;

		ReverseIterator m_rfirst; // Position = m_first - 1
		ReverseIterator m_rmiddle; // Position = m_middle == m_rfirst + (m_r - 1)
		ReverseIterator m_rlast; // Position = m_last - 1

		bool m_is_initialized;
	};
}

namespace kanalysis::arithmetic
{
	KANALYSIS_INLINE IndexVector::IndexVector()
		: m_n(0)
		, m_r(0)
		, m_first(nullptr)
		, m_middle(nullptr)
		, m_last(nullptr)
		, m_is_initialized(false)
	{}

	KANALYSIS_INLINE IndexVector::IndexVector(Index n, Index r)
		: m_n(n)
		, m_r(r)
		, m_indicies(n)
		, m_is_initialized(true)
	{
		update_iterator();
	}

	KANALYSIS_INLINE IndexVector::IndexVector(Index n)
		: IndexVector(n, n / 2)
	{}

	KANALYSIS_INLINE IndexVector::IndexVector(const IndexVector& other)
		: m_n(other.m_n)
		, m_r(other.m_r)
		, m_indicies(other.m_indicies)
		, m_is_initialized(true)
	{
		update_iterator();
	}

	KANALYSIS_INLINE IndexVector::IndexVector(IndexVector&& other) noexcept
		: m_n(other.m_n)
		, m_r(other.m_r)
		, m_indicies(std::move(other.m_indicies))
		, m_is_initialized(true)
	{
		update_iterator();
	}

	KANALYSIS_INLINE IndexVector& IndexVector::operator=(const IndexVector& rhs)
	{
		if (this == &rhs) return *this;
		m_n = rhs.m_n;
		m_r = rhs.m_r;
		m_indicies = rhs.m_indicies;
		m_is_initialized = true;
		update_iterator();
		return *this;
	}

	KANALYSIS_INLINE IndexVector& IndexVector::operator=(IndexVector&& rhs) noexcept
	{
		if (this == &rhs) return *this;
		m_n = rhs.m_n;
		m_r = rhs.m_r;
		m_indicies = std::move(rhs.m_indicies);
		m_is_initialized = true;
		update_iterator();
		return *this;
	}

	KANALYSIS_INLINE void IndexVector::set_middle(Index r)
	{
		assert(m_is_initialized);
		assert(r >= -1);
		assert(r <= m_n);

		m_r = r;
		m_middle = m_first + m_r;
		m_rmiddle = ReverseIterator(m_middle);
	}

	KANALYSIS_INLINE void IndexVector::print() const
	{
		assert(m_is_initialized);

		Iterator first = m_first;
		Iterator middle = m_middle;
		Iterator last = m_last;

		std::cout << "[ ";
		for (; first != middle; ++first)
		{
			std::cout << *first << ' ';
		}
		std::cout << "] [ ";
		for (; middle != last; ++middle)
		{
			std::cout << *middle << ' ';
		}
		std::cout << "]\n";
	}

	KANALYSIS_INLINE Index IndexVector::n() const
	{
		assert(m_is_initialized);
		return m_n;
	}

	KANALYSIS_INLINE Index IndexVector::r() const
	{
		assert(m_is_initialized);
		return m_r;
	}

	KANALYSIS_INLINE std::vector<Index> IndexVector::indicies() const
	{
		assert(m_is_initialized);
		return m_indicies;
	}

	KANALYSIS_INLINE IndexVector::Iterator IndexVector::begin() const
	{
		assert(m_is_initialized);
		return m_first;
	}

	KANALYSIS_INLINE IndexVector::Iterator IndexVector::middle() const
	{
		assert(m_is_initialized);
		return m_middle;
	}

	KANALYSIS_INLINE IndexVector::Iterator IndexVector::end() const
	{
		assert(m_is_initialized);
		return m_last;
	}

	KANALYSIS_INLINE IndexVector::ReverseIterator IndexVector::rbegin() const
	{
		assert(m_is_initialized);
		return m_rfirst;
	}

	KANALYSIS_INLINE IndexVector::ReverseIterator IndexVector::rmiddle() const
	{
		assert(m_is_initialized);
		return m_rmiddle;
	}

	KANALYSIS_INLINE IndexVector::ReverseIterator IndexVector::rend() const
	{
		assert(m_is_initialized);
		return m_rlast;
	}

	KANALYSIS_INLINE void IndexVector::update_iterator()
	{
		assert(m_is_initialized);
		assert(m_r >= -1);
		assert(m_r <= m_n);

		m_first = m_indicies.data();
		m_middle = m_first + m_r;
		m_last = m_first + m_n;

		m_rfirst = ReverseIterator(m_last);
		m_rmiddle = ReverseIterator(m_middle);
		m_rlast = ReverseIterator(m_first);
	}
} // namespace kanalysis::arithmetic