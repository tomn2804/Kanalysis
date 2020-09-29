#pragma once

#include "include/config.h"

#include <numeric>
#include "include/arithmetic/iterator_base.h"
#include "include/arithmetic/permutation/next_permutation.h"
#include "include/arithmetic/permutation/permutation.h"

namespace kanalysis::arithmetic
{
	template<typename DerivedType>
	class PermutationIteratorBase : public IteratorBase<DerivedType>
	{
	protected:
		using IteratorBase = IteratorBase<DerivedType>;
	public:
		PermutationIteratorBase() = default;
		PermutationIteratorBase(int n, int r, UInt k, UInt p);
		PermutationIteratorBase(int n, int r, UInt k);
		PermutationIteratorBase(int n, int r);
		PermutationIteratorBase(int n);

		void next();
		void prev();

		void reset(int r, UInt k, UInt p);
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
	PermutationIteratorBase<DerivedType>::PermutationIteratorBase(int n, int r, UInt k, UInt p)
		: IteratorBase(n, r, k, p)
	{
		update_state();
	}

	template<typename DerivedType>
	PermutationIteratorBase<DerivedType>::PermutationIteratorBase(int n, int r, UInt k)
		: PermutationIteratorBase(n, r, k, permutation::factorial(n, r))
	{}

	template<typename DerivedType>
	PermutationIteratorBase<DerivedType>::PermutationIteratorBase(int n, int r)
		: PermutationIteratorBase(n, r, 0)
	{}

	template<typename DerivedType>
	PermutationIteratorBase<DerivedType>::PermutationIteratorBase(int n)
		: PermutationIteratorBase(n, n)
	{}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::next()
	{
		assert(IteratorBase::m_is_initialized);
		permutation::next_partial_permutation(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), IteratorBase::m_indicies.end());
		IteratorBase::increment_current_index();
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::prev()
	{
		assert(IteratorBase::m_is_initialized);
		permutation::prev_partial_permutation(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), IteratorBase::m_indicies.end());
		IteratorBase::decrement_current_index();
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::reset(int r, UInt k, UInt p)
	{
		assert(IteratorBase::m_is_initialized);
		permutation::detail::validate(IteratorBase::n(), r, k, p);
		IteratorBase::update_state(r, k, p);
		update_state();
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::reset(int r, UInt k)
	{
		assert(IteratorBase::m_is_initialized);
		reset(r, k, permutation::factorial(IteratorBase::n(), r));
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::reset(int r)
	{
		assert(IteratorBase::m_is_initialized);
		reset(r, 0);
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::reset()
	{
		assert(IteratorBase::m_is_initialized);
		reset(IteratorBase::n());
	}

	template<typename DerivedType>
	void PermutationIteratorBase<DerivedType>::update_state()
	{
		assert(IteratorBase::m_is_initialized);
		permutation::detail::validate(IteratorBase::n(), IteratorBase::r(), IteratorBase::m_current_index, IteratorBase::m_stop_index);

		std::iota(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.end(), 0);
		for (UInt i = 0; i < IteratorBase::m_current_index; ++i)
		{
			permutation::next_partial_permutation(IteratorBase::m_indicies.begin(), IteratorBase::m_indicies.middle(), IteratorBase::m_indicies.end());
		}
	}
} // namespace kanalysis::arithmetic
