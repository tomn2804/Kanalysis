#pragma once

#include "include/config.h"

#include "include/arithmetic/col_functor.h"

namespace kanalysis::arithmetic
{
	///
	/// \brief A wrapper class for ColFunctor that handles 2 matrices simultaneously instead of 1.
	///
	/// \sa class ColFunctor
	///
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
		void operator()(const ContainerA& lhs_x, const ContainerB& rhs_x); // To-Do: Return std::tuple

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
	///
	/// \brief A constructor.
	///
	/// \details The rhs matrix will copy from lhs matrix.
	///
	/// \param lhs_matrix A lhs matrix to be iterated.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename Derived>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<Derived>& lhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(lhs_matrix)
	{}

	///
	/// \overload TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<Derived>& lhs_matrix)
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename Derived>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(DenseBase<Derived>& lhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(lhs_matrix)
	{}

	///
	/// \brief A constructor.
	///
	/// \param lhs_matrix A lhs matrix to be iterated.
	/// \param rhs_matrix A rhs matrix to be iterated.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename DerivedA, typename DerivedB>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<DerivedA>& lhs_matrix, const DenseBase<DerivedB>& rhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(rhs_matrix)
	{}

	///
	/// \overload TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(const DenseBase<DerivedA>& lhs_matrix, const DenseBase<DerivedB>& rhs_matrix)
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename DerivedA, typename DerivedB>
	TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::TwoHandsSideColFunctor(DenseBase<DerivedA>& lhs_matrix, DenseBase<DerivedB>& rhs_matrix)
		: m_lhs(lhs_matrix)
		, m_rhs(rhs_matrix)
	{}

	///
	/// \brief A call operator for each iterations generated by a \a for_each loop.
	///
	/// \param lhs_x A vector of indices to re-arrange the lhs matrix.
	/// \param rhs_x A vector of indices to re-arrange the rhs matrix.
	///
	/// \sa MatrixType ColFunctor<MatrixType>::operator()(const Container& x)
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename ContainerA, typename ContainerB>
	void TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::operator()(const ContainerA& lhs_x, const ContainerB& rhs_x)
	{
		m_lhs(lhs_x);
		m_rhs(rhs_x);
	}

	///
	/// \brief Set the initial state of the lhs and rhs underlying matrices to the given the vector of indices.
	///
	/// \param lhs_x A vector of indices to set the lhs matrix.
	/// \param rhs_x A vector of indices to set the rhs matrix.
	///
	/// \sa void ColFunctor<MatrixType>::reset(const Container& x)
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	template<typename ContainerA, typename ContainerB>
	void TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::reset(const ContainerA& lhs_x, const ContainerB& rhs_x)
	{
		m_lhs.reset(lhs_x);
		m_rhs.reset(rhs_x);
	}

	///
	/// \return The lhs ColFunctor.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	const ColFunctor<LhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::lhs() const
	{
		return m_lhs;
	}

	///
	/// \return The const casted lhs ColFunctor.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	ColFunctor<LhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::const_cast_lhs() const
	{
		return m_lhs;
	}

	///
	/// \return The rhs ColFunctor.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	const ColFunctor<RhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::rhs() const
	{
		return m_rhs;
	}

	///
	/// \return The const casted rhs ColFunctor.
	///
	template<typename LhsMatrixType, typename RhsMatrixType>
	ColFunctor<RhsMatrixType>& TwoHandsSideColFunctor<LhsMatrixType, RhsMatrixType>::const_cast_rhs() const
	{
		return m_rhs;
	}
} // namespace kanalysis::arithmetic
