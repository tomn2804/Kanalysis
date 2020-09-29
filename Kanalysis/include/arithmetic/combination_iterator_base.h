#pragma once

#include "include/config.h"

#include <numeric>
#include "include/arithmetic/combination/combination.h"
#include "include/arithmetic/combination/next_combination.h"
#include "include/arithmetic/iterator_base.h"
#include "include/utils/utils.h"

namespace kanalysis::arithmetic
{
	template<typename DerivedType>
	class CombinationIteratorBase : public IteratorBase<DerivedType>
	{
	protected:
		using IteratorBase = IteratorBase<DerivedType>;
	public:
		CombinationIteratorBase() = default;
		CombinationIteratorBase(int n, int r, UInt k, UInt c);
		CombinationIteratorBase(int n, int r, UInt k);
		CombinationIteratorBase(int n, int r);
		CombinationIteratorBase(int n);

		void next();
		void prev();

		void reset(int r, UInt k, UInt c);
		void reset(int r, UInt k);
		void reset(int r);
		void reset();
	private:
		void update_state();
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename DerivedType>
	CombinationIteratorBase<DerivedType>::CombinationIteratorBase(int n, int r, UInt k, UInt c)
		: IteratorBase(n, r, k, c)
	{
		update_state();
	}

	template<typename DerivedType>
	CombinationIteratorBase<DerivedType>::CombinationIteratorBase(int n, int r, UInt k)
		: CombinationIteratorBase(n, r, k, combination::choose(n, r))
	{}

	template<typename DerivedType>
	CombinationIteratorBase<DerivedType>::CombinationIteratorBase(int n, int r)
		: CombinationIteratorBase(n, r, 0)
	{}

	template<typename DerivedType>
	CombinationIteratorBase<DerivedType>::CombinationIteratorBase(int n)
		: CombinationIteratorBase(n, n)
	{}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::next()
	{
		assert(IteratorBase::m_is_initialized);
		combination::next_combination(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), IteratorBase::m_indicies.end());
		IteratorBase::increment_current_index();
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::prev()
	{
		assert(IteratorBase::m_is_initialized);
		combination::prev_combination(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), IteratorBase::m_indicies.end());
		IteratorBase::decrement_current_index();
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::reset(int r, UInt k, UInt c)
	{
		assert(IteratorBase::m_is_initialized);
		combination::detail::validate(IteratorBase::n(), r, k, c);
		IteratorBase::update_state(r, k, c);
		update_state();
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::reset(int r, UInt k)
	{
		assert(IteratorBase::m_is_initialized);
		reset(r, k, combination::choose(IteratorBase::n(), r));
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::reset(int r)
	{
		assert(IteratorBase::m_is_initialized);
		reset(r, 0);
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::reset()
	{
		assert(IteratorBase::m_is_initialized);
		reset(IteratorBase::n());
	}

	template<typename DerivedType>
	void CombinationIteratorBase<DerivedType>::update_state()
	{
		assert(IteratorBase::m_is_initialized);
		combination::detail::validate(IteratorBase::n(), IteratorBase::r(), IteratorBase::m_current_index, IteratorBase::m_stop_index);

		if (IteratorBase::m_current_index == 0)
		{
			std::iota(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.end(), 0);
			return;
		}

		combination::combinations(IteratorBase::n(), IteratorBase::r(), IteratorBase::m_current_index, IteratorBase::m_indicies.begin());

		IndexVector::Iterator m = IteratorBase::m_indicies.middle();
		for (Index i = 0; i < IteratorBase::n(); ++i)
		{
			if (!utils::contains(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), i))
			{
				*m = i;
				++m;
			}
		}

		assert(m == IteratorBase::m_indicies.end());
	}
} // namespace kanalysis::arithmetic
