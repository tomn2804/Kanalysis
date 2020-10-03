#pragma once

#include "include/config.h"

#include "include/arithmetic/col_functor.h"

namespace kanalysis::arithmetic
{
	template<typename LhsMatrixType, typename RhsMatrixType>
	class TwoHandsSideColFunctor
	{
	public:
		TwoHandsSideColFunctor() = default;

		template<typename Derived>
		TwoHandsSideColFunctor(const DenseBase<Derived>& lhs_matrix);

		template<typename Derived>
		TwoHandsSideColFunctor(DenseBase<Derived>& lhs_matrix);

		template<typename DerivedA, typename DerivedB>
		TwoHandsSideColFunctor(const DenseBase<DerivedA>& lhs_matrix, const DenseBase<DerivedB>& rhs_matrix);

		template<typename DerivedA, typename DerivedB>
		TwoHandsSideColFunctor(DenseBase<DerivedA>& lhs_matrix, DenseBase<DerivedB>& rhs_matrix);

		template<typename ContainerA, typename ContainerB>
		void operator()(const ContainerA& lhs_x, const ContainerB& rhs_x);

		template<typename ContainerA, typename ContainerB>
		void reset(const ContainerA& lhs_x, const ContainerB& rhs_x);

		const ColFunctor<LhsMatrixType>& lhs() const;
		ColFunctor<LhsMatrixType>& const_cast_lhs() const;

		const ColFunctor<RhsMatrixType>& rhs() const;
		ColFunctor<RhsMatrixType>& const_cast_rhs() const;
	private:
		ColFunctor<LhsMatrixType> m_lhs;
		ColFunctor<RhsMatrixType> m_rhs;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename Derived>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<Derived>& lhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(lhs_matrix)
	{}

	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename Derived>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(DenseBase<Derived>& lhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(lhs_matrix)
	{}

	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename DerivedA, typename DerivedB>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<DerivedA>& lhs_matrix, const DenseBase<DerivedB>& rhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(rhs_matrix)
	{}

	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename DerivedA, typename DerivedB>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(DenseBase<DerivedA>& lhs_matrix, DenseBase<DerivedB>& rhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(rhs_matrix)
	{}

	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename ContainerA, typename ContainerB>
	void TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::operator()(const ContainerA& lhs_x, const ContainerB& rhs_x)
	{
		m_lhs(lhs_x);
		m_rhs(rhs_x);
	}

	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename ContainerA, typename ContainerB>
	void TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::reset(const ContainerA& lhs_x, const ContainerB& rhs_x)
	{
		m_lhs.reset(lhs_x);
		m_rhs.reset(rhs_x);
	}

	template<typename LhsMatrixType, typename RhsMatrixType>
	const ColFunctor<LhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::lhs() const
	{
		return m_lhs;
	}

	template<typename LhsMatrixType, typename RhsMatrixType>
	ColFunctor<LhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::const_cast_lhs() const
	{
		return const_cast<ColFunctor<LhsMatrixType>&>(m_lhs);
	}

	template<typename LhsMatrixType, typename RhsMatrixType>
	const ColFunctor<RhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::rhs() const
	{
		return m_rhs;
	}

	template<typename LhsMatrixType, typename RhsMatrixType>
	ColFunctor<RhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::const_cast_rhs() const
	{
		return const_cast<ColFunctor<RhsMatrixType>&>(m_rhs);
	}
} // namespace kanalysis::arithmetic
