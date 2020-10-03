#pragma once

#include "include/config.h"

#include "include/utils/crtp_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct ComputeHolderTraits;

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
		HouseholderQR<Matrix> m_householder_qr = HouseholderQR<Matrix>(m_std_x.rows(), m_std_x.cols());

		bool m_is_initialized = false;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(Index rows, Index cols)
		: m_std_x(rows, cols)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<Derived>& std_x)
		: m_std_x(std_x.derived())
		, m_householder_qr(m_std_x)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename Derived>
	void ComputeHolderBase<DerivedType>::compute(const DenseBase<Derived>& std_x)
	{
		m_std_x = std_x.derived();
		m_householder_qr.compute(m_std_x);
		m_is_initialized = true;
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	void ComputeHolderBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		Base::derived().standardize(x, out);
	}

	template<typename DerivedType>
	template<typename Derived>
	Matrix ComputeHolderBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	{
		return Base::derived().standardize(x);
	}

	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) ComputeHolderBase<DerivedType>::compute_holder(const DenseBase<Derived>& std_x)
	{
		return Base::derived().compute_holder(std_x);
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::rows() const
	{
		return m_std_x.rows();
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::cols() const
	{
		return m_std_x.cols();
	}

	template<typename DerivedType>
	const typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::std_x() const
	{
		assert(m_is_initialized);
		return m_std_x;
	}

	template<typename DerivedType>
	typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::const_cast_std_x() const
	{
		assert(m_is_initialized);
		return const_cast<MatrixType&>(m_std_x);
	}

	template<typename DerivedType>
	const HouseholderQR<Matrix>& ComputeHolderBase<DerivedType>::householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}

	template<typename DerivedType>
	HouseholderQR<Matrix>& ComputeHolderBase<DerivedType>::const_cast_householder_qr() const
	{
		assert(m_is_initialized);
		return m_householder_qr;
	}
} // namespace kanalysis::stats
