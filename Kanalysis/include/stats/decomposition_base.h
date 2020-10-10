#pragma once

#include "include/config.h"

#include "include/utils/crtp_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct QrDecompositionTraits;

	///
	/// \brief A CRTP base class for \a Decomposition and \a DecompositionWeight .
	///
	template<typename DerivedType>
	class DecompositionBase : public utils::CrtpBase<DerivedType>
	{
	protected:
		using Base = utils::CrtpBase<DerivedType>;
		using QrDecompositionTraits = QrDecompositionTraits<DerivedType>;
	public:
		using MatrixType = typename QrDecompositionTraits::MatrixType;
		using ArrayType = typename QrDecompositionTraits::ArrayType;

		DecompositionBase(Index rows, Index cols);

		template<typename Derived>
		DecompositionBase(const DenseBase<Derived>& std_x);

		template<typename Derived>
		DecompositionBase(DenseBase<Derived>& std_x);

		template<typename Derived>
		void compute(const DenseBase<Derived>& std_x);

		template<typename DerivedA, typename DerivedB>
		static void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Matrix standardize(const DenseBase<Derived>& x);

		template<typename Derived>
		decltype(auto) decomposition(const DenseBase<Derived>& std_x);

		Index rows() const;
		Index cols() const;

		const MatrixType& std_x() const;
		MatrixType& const_cast_std_x() const;

		const HouseholderQR<Matrix>& householder_qr() const;
		HouseholderQR<Matrix>& const_cast_householder_qr() const;
	protected:
		DecompositionBase() = default;

		MatrixType m_std_x;
		HouseholderQR<Matrix> m_householder_qr = HouseholderQR<Matrix>(m_std_x.rows(), m_std_x.cols()); // Decomposition of \a m_std_x .

		bool m_is_initialized = false;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A constructor.
	///
	/// \param rows Number of rows for the underlying matrix.
	/// \param cols Number of columns for the underlying matrix.
	///
	template<typename DerivedType>
	DecompositionBase<DerivedType>::DecompositionBase(Index rows, Index cols)
		: m_std_x(rows, cols)
	{}

	///
	/// \brief A constructor.
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	DecompositionBase<DerivedType>::DecompositionBase(const DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	///
	/// \overload DecompositionBase<DerivedType>::DecompositionBase(const DenseBase<Derived>& std_x)
	///
	template<typename DerivedType>
	template<typename Derived>
	DecompositionBase<DerivedType>::DecompositionBase(DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	///
	/// \brief Set the underlying matrix to \a std_x and compute it's qr.
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	void DecompositionBase<DerivedType>::compute(const DenseBase<Derived>& std_x)
	{
		m_std_x = std_x.derived();
		m_householder_qr.compute(m_std_x);
		m_is_initialized = true;
	}

	///
	/// \brief A conversion function that will standardize the values stored in \a x .
	/// The standardized values can be used for all other objects within the \a stats namespace.
	///
	/// \param x A matrix or vector.
	/// \param out The standardized values are outputted here.
	///
	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	void DecompositionBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		Base::derived().standardize(x, out);
	}

	///
	/// \brief A conversion function that will standardize the values stored in \a x .
	/// The standardized values can be used for all other objects within the \a stats namespace.
	///
	/// \details This function does nothing beside prodiving a consistent interface for \a DecompositionWeight and semantic reasons.
	///
	/// \param x A matrix or vector.
	/// \return A matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	Matrix DecompositionBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	{
		return Base::derived().standardize(x);
	}

	///
	/// \brief A factory function.
	///
	/// \param std_x A matrix with standardized values.
	/// \return A new \a Decomposition where \a MatrixType is equal to \a std_x type, and \a ArrayType remains the same.
	///
	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) DecompositionBase<DerivedType>::decomposition(const DenseBase<Derived>& std_x)
	{
		return Base::derived().decomposition(std_x);
	}

	///
	/// \return The number of rows of the underlying matrix.
	///
	template<typename DerivedType>
	Index DecompositionBase<DerivedType>::rows() const
	{
		return m_std_x.rows();
	}

	///
	/// \return The number of columns of the underlying matrix.
	///
	template<typename DerivedType>
	Index DecompositionBase<DerivedType>::cols() const
	{
		return m_std_x.cols();
	}

	///
	/// \return The underlying matrix.
	///
	template<typename DerivedType>
	const typename DecompositionBase<DerivedType>::MatrixType& DecompositionBase<DerivedType>::std_x() const
	{
		assert(m_is_initialized);
		return m_std_x;
	}

	///
	/// \return The const casted underlying matrix.
	///
	template<typename DerivedType>
	typename DecompositionBase<DerivedType>::MatrixType& DecompositionBase<DerivedType>::const_cast_std_x() const
	{
		assert(m_is_initialized);
		return const_cast<MatrixType&>(m_std_x);
	}

	///
	/// \return The underlying qr.
	///
	template<typename DerivedType>
	const HouseholderQR<Matrix>& DecompositionBase<DerivedType>::householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}

	///
	/// \return The const casted underlying qr.
	///
	template<typename DerivedType>
	HouseholderQR<Matrix>& DecompositionBase<DerivedType>::const_cast_householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}
} // namespace kanalysis::stats
