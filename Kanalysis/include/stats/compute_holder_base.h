#pragma once

#include "include/config.h"

#include "include/utils/crtp_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct ComputeHolderTraits;

	///
	/// \brief A CRTP base class for \a ComputeHolder and \a ComputeHolderWeight .
	///
	template<typename DerivedType>
	class ComputeHolderBase : public utils::CrtpBase<DerivedType>
	{
	protected:
		using Base = utils::CrtpBase<DerivedType>;
		using ComputeHolderTraits = ComputeHolderTraits<DerivedType>;
	public:
		using MatrixType = typename ComputeHolderTraits::MatrixType;
		using ArrayType = typename ComputeHolderTraits::ArrayType;

		ComputeHolderBase(Index rows, Index cols);

		template<typename Derived>
		ComputeHolderBase(const DenseBase<Derived>& std_x);

		template<typename Derived>
		ComputeHolderBase(DenseBase<Derived>& std_x);

		template<typename Derived>
		void compute(const DenseBase<Derived>& std_x);

		template<typename DerivedA, typename DerivedB>
		static void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Matrix standardize(const DenseBase<Derived>& x);

		template<typename Derived>
		decltype(auto) compute_holder(const DenseBase<Derived>& std_x);

		Index rows() const;
		Index cols() const;

		const MatrixType& std_x() const;
		MatrixType& const_cast_std_x() const;

		const HouseholderQR<Matrix>& householder_qr() const;
		HouseholderQR<Matrix>& const_cast_householder_qr() const;
	protected:
		ComputeHolderBase() = default;

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
	ComputeHolderBase<DerivedType>::ComputeHolderBase(Index rows, Index cols)
		: m_std_x(rows, cols)
	{}

	///
	/// \brief A constructor.
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	///
	/// \overload ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<Derived>& std_x)
	///
	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	///
	/// \brief Set the underlying matrix to \a std_x and compute it's decomposition.
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	void ComputeHolderBase<DerivedType>::compute(const DenseBase<Derived>& std_x)
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
	void ComputeHolderBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		Base::derived().standardize(x, out);
	}

	///
	/// \brief A conversion function that will standardize the values stored in \a x .
	/// The standardized values can be used for all other objects within the \a stats namespace.
	///
	/// \details This function does nothing beside prodiving a consistent interface for \a ComputeHolderWeight and semantic reasons.
	///
	/// \param x A matrix or vector.
	/// \return A matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	Matrix ComputeHolderBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	{
		return Base::derived().standardize(x);
	}

	///
	/// \brief A factory function.
	///
	/// \param std_x A matrix or vector.
	/// \return A new \a ComputeHolder where \a MatrixType is equal to \a std_x type, and \a ArrayType remains the same.
	///
	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) ComputeHolderBase<DerivedType>::compute_holder(const DenseBase<Derived>& std_x)
	{
		return Base::derived().compute_holder(std_x);
	}

	///
	/// \return The number of rows of the underlying matrix.
	///
	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::rows() const
	{
		return m_std_x.rows();
	}

	///
	/// \return The number of columns of the underlying matrix.
	///
	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::cols() const
	{
		return m_std_x.cols();
	}

	///
	/// \return The underlying matrix.
	///
	template<typename DerivedType>
	const typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::std_x() const
	{
		assert(m_is_initialized);
		return m_std_x;
	}

	///
	/// \return The const casted underlying matrix.
	///
	template<typename DerivedType>
	typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::const_cast_std_x() const
	{
		assert(m_is_initialized);
		return const_cast<MatrixType&>(m_std_x);
	}

	///
	/// \return The underlying decomposition.
	///
	template<typename DerivedType>
	const HouseholderQR<Matrix>& ComputeHolderBase<DerivedType>::householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}

	///
	/// \return The const casted underlying decomposition.
	///
	template<typename DerivedType>
	HouseholderQR<Matrix>& ComputeHolderBase<DerivedType>::const_cast_householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}
} // namespace kanalysis::stats
