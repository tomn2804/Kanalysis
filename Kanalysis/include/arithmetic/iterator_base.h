#pragma once

#include "include/config.h"

#include "include/arithmetic/index_vector.h"
#include "include/utils/crtp_base.h"

namespace kanalysis::arithmetic
{
	template<typename DerivedType>
	class IteratorBase : public utils::CrtpBase<DerivedType>
	{
	protected:
		using CrtpBase = utils::CrtpBase<DerivedType>;
		using Iterator = IndexVector::Iterator;
		using ReverseIterator = IndexVector::ReverseIterator;
	public:
		bool has_next() const;
		void next();

		bool has_prev() const;
		void prev();

		void reset(int r, UInt start_index, UInt stop_index);
		void reset(int r, UInt start_index);
		void reset(int r);
		void reset();

		Index n() const;
		Index r() const;

		const IndexVector& indicies() const;

		UInt current_index() const;
		UInt stop_index() const;
	protected:
		IteratorBase();
		IteratorBase(int n, int r, UInt start_index, UInt stop_index);
		IteratorBase(int n, UInt start_index, UInt stop_index);

		void increment_current_index();
		void decrement_current_index();

		void update_state(int r, UInt start_index, UInt stop_index);

		IndexVector m_indicies;

		UInt m_current_index;
		UInt m_stop_index;

		bool m_is_initialized;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename DerivedType>
	bool IteratorBase<DerivedType>::has_next() const
	{
		assert(m_is_initialized);
		return m_current_index < m_stop_index;
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::next()
	{
		assert(has_next());
		CrtpBase::derived().next();
	}

	template<typename DerivedType>
	bool IteratorBase<DerivedType>::has_prev() const
	{
		assert(m_is_initialized);
		return (m_current_index < m_stop_index) ? true : false;
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::prev()
	{
		assert(has_prev());
		CrtpBase::derived().prev();
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::reset(int r, UInt start_index, UInt stop_index)
	{
		CrtpBase::derived().reset(r, start_index, stop_index);
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::reset(int r, UInt start_index)
	{
		CrtpBase::derived().reset(r, start_index);
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::reset(int r)
	{
		CrtpBase::derived().reset(r);
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::reset()
	{
		CrtpBase::derived().reset();
	}

	template<typename DerivedType>
	Index IteratorBase<DerivedType>::n() const
	{
		assert(m_is_initialized);
		return m_indicies.n();
	}

	template<typename DerivedType>
	Index IteratorBase<DerivedType>::r() const
	{
		assert(m_is_initialized);
		return m_indicies.r();
	}

	template<typename DerivedType>
	const IndexVector& IteratorBase<DerivedType>::indicies() const
	{
		assert(m_is_initialized);
		return m_indicies;
	}

	template<typename DerivedType>
	UInt IteratorBase<DerivedType>::current_index() const
	{
		assert(m_is_initialized);
		return m_current_index;
	}

	template<typename DerivedType>
	UInt IteratorBase<DerivedType>::stop_index() const
	{
		assert(m_is_initialized);
		return m_stop_index;
	}

	template<typename DerivedType>
	IteratorBase<DerivedType>::IteratorBase()
		: m_current_index(0)
		, m_stop_index(0)
		, m_is_initialized(false)
	{}

	template<typename DerivedType>
	IteratorBase<DerivedType>::IteratorBase(int n, UInt start_index, UInt stop_index)
		: m_indicies(n, n)
		, m_current_index(start_index)
		, m_stop_index(stop_index)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	IteratorBase<DerivedType>::IteratorBase(int n, int r, UInt start_index, UInt stop_index)
		: m_indicies(n, r)
		, m_current_index(start_index)
		, m_stop_index(stop_index)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::increment_current_index()
	{
		assert(m_is_initialized);
		++m_current_index;
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::decrement_current_index()
	{
		assert(m_is_initialized);
		m_current_index = (m_current_index != 0) ? --m_current_index : m_stop_index;
	}

	template<typename DerivedType>
	void IteratorBase<DerivedType>::update_state(int r, UInt start_index, UInt stop_index)
	{
		assert(m_is_initialized);
		assert(r >= 0);
		assert(r <= n());
		assert(start_index < stop_index);

		m_indicies.set_middle(r);
		m_current_index = start_index;
		m_stop_index = stop_index;
	}
} // namespace kanalysis::arithmetic
