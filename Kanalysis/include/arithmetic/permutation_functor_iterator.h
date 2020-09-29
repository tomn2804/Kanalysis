#pragma once

#include "include/config.h"

#include "include/arithmetic/permutation_iterator_base.h"

namespace kanalysis::arithmetic
{
	template<typename FunctorType>
	class PermutationFunctorIterator : public PermutationIteratorBase<PermutationFunctorIterator<FunctorType>>
	{
	protected:
		using PermutationIteratorBase = PermutationIteratorBase<PermutationFunctorIterator<FunctorType>>;
	public:
		PermutationFunctorIterator() = default;
		PermutationFunctorIterator(const FunctorType& functor, int n, int r, UInt k, UInt p);
		PermutationFunctorIterator(const FunctorType& functor, int n, int r, UInt k);
		PermutationFunctorIterator(const FunctorType& functor, int n, int r);
		PermutationFunctorIterator(const FunctorType& functor, int n);

		auto next();
		auto prev();

		void reset(int r, UInt k, UInt p);
		void reset(int r, UInt k);
		void reset(int r);
		void reset();
	private:
		FunctorType m_functor;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename FunctorType>
	PermutationFunctorIterator<FunctorType>::PermutationFunctorIterator(const FunctorType& functor, int n, int r, UInt k, UInt p)
		: PermutationIteratorBase(n, r, k, p)
		, m_functor(functor)
	{}

	template<typename FunctorType>
	PermutationFunctorIterator<FunctorType>::PermutationFunctorIterator(const FunctorType& functor, int n, int r, UInt k)
		: PermutationFunctorIterator(functor, n, r, k, permutation::factorial(n, r))
	{}

	template<typename FunctorType>
	PermutationFunctorIterator<FunctorType>::PermutationFunctorIterator(const FunctorType& functor, int n, int r)
		: PermutationFunctorIterator(functor, n, r, 0)
	{}

	template<typename FunctorType>
	PermutationFunctorIterator<FunctorType>::PermutationFunctorIterator(const FunctorType& functor, int n)
		: PermutationFunctorIterator(functor, n, n)
	{}

	template<typename FunctorType>
	auto PermutationFunctorIterator<FunctorType>::next()
	{
		assert(PermutationIteratorBase::m_is_initialized);
		PermutationIteratorBase::next();
		return m_functor(PermutationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	auto PermutationFunctorIterator<FunctorType>::prev()
	{
		assert(PermutationIteratorBase::m_is_initialized);
		PermutationIteratorBase::prev();
		return m_functor(PermutationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	void PermutationFunctorIterator<FunctorType>::reset(int r, UInt k, UInt p)
	{
		assert(PermutationIteratorBase::m_is_initialized);
		PermutationIteratorBase::reset(r, k, p);
		m_functor(PermutationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	void PermutationFunctorIterator<FunctorType>::reset(int r, UInt k)
	{
		assert(PermutationIteratorBase::m_is_initialized);
		reset(r, k, permutation::factorial(PermutationIteratorBase::n(), r));
	}

	template<typename FunctorType>
	void PermutationFunctorIterator<FunctorType>::reset(int r)
	{
		assert(PermutationIteratorBase::m_is_initialized);
		reset(r, 0);
	}

	template<typename FunctorType>
	void PermutationFunctorIterator<FunctorType>::reset()
	{
		assert(PermutationIteratorBase::m_is_initialized);
		reset(PermutationIteratorBase::n());
	}
} // namespace kanalysis::arithmetic
