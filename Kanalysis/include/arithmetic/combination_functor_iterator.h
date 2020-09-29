#pragma once

#include "include/config.h"

#include "include/arithmetic/combination_iterator_base.h"

namespace kanalysis::arithmetic
{
	template<typename FunctorType>
	class CombinationFunctorIterator : public CombinationIteratorBase<CombinationFunctorIterator<FunctorType>>
	{
	protected:
		using CombinationIteratorBase = CombinationIteratorBase<CombinationFunctorIterator<FunctorType>>;
	public:
		CombinationFunctorIterator() = default;
		CombinationFunctorIterator(const FunctorType& functor, int n, int r, UInt k, UInt c);
		CombinationFunctorIterator(const FunctorType& functor, int n, int r, UInt k);
		CombinationFunctorIterator(const FunctorType& functor, int n, int r);
		CombinationFunctorIterator(const FunctorType& functor, int n);

		auto next();
		auto prev();

		void reset(int r, UInt k, UInt c);
		void reset(int r, UInt k);
		void reset(int r);
		void reset();

		const FunctorType& functor() const;
	private:
		FunctorType m_functor;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename FunctorType>
	CombinationFunctorIterator<FunctorType>::CombinationFunctorIterator(const FunctorType& functor, int n, int r, UInt k, UInt c)
		: CombinationIteratorBase(n, r, k, c)
		, m_functor(functor)
	{}

	template<typename FunctorType>
	CombinationFunctorIterator<FunctorType>::CombinationFunctorIterator(const FunctorType& functor, int n, int r, UInt k)
		: CombinationFunctorIterator(functor, n, r, k, combination::choose(n, r))
	{}

	template<typename FunctorType>
	CombinationFunctorIterator<FunctorType>::CombinationFunctorIterator(const FunctorType& functor, int n, int r)
		: CombinationFunctorIterator(functor, n, r, 0)
	{}

	template<typename FunctorType>
	CombinationFunctorIterator<FunctorType>::CombinationFunctorIterator(const FunctorType& functor, int n)
		: CombinationFunctorIterator(functor, n, n)
	{}

	template<typename FunctorType>
	auto CombinationFunctorIterator<FunctorType>::next()
	{
		assert(CombinationIteratorBase::m_is_initialized);
		CombinationIteratorBase::next();
		return m_functor(CombinationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	auto CombinationFunctorIterator<FunctorType>::prev()
	{
		assert(CombinationIteratorBase::m_is_initialized);
		CombinationIteratorBase::prev();
		return m_functor(CombinationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	void CombinationFunctorIterator<FunctorType>::reset(int r, UInt k, UInt c)
	{
		assert(CombinationIteratorBase::m_is_initialized);
		CombinationIteratorBase::reset(r, k, c);
		m_functor.reset(CombinationIteratorBase::const_derived());
	}

	template<typename FunctorType>
	void CombinationFunctorIterator<FunctorType>::reset(int r, UInt k)
	{
		assert(CombinationIteratorBase::m_is_initialized);
		reset(r, k, combination::choose(CombinationIteratorBase::n(), r));
	}

	template<typename FunctorType>
	void CombinationFunctorIterator<FunctorType>::reset(int r)
	{
		assert(CombinationIteratorBase::m_is_initialized);
		reset(r, 0);
	}

	template<typename FunctorType>
	void CombinationFunctorIterator<FunctorType>::reset()
	{
		assert(CombinationIteratorBase::m_is_initialized);
		reset(CombinationIteratorBase::n());
	}

	template<typename FunctorType>
	const FunctorType& CombinationFunctorIterator<FunctorType>::functor() const
	{
		assert(CombinationIteratorBase::m_is_initialized);
		return m_functor;
	}
} // namespace kanalysis::arithmetic
