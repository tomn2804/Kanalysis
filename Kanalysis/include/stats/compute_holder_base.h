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
		ComputeHolderBase(const DenseBase<Derived>& std_matrix);

		template<typename Derived>
		ComputeHolderBase(DenseBase<Derived>& std_matrix);

		template<typename Derived>
		void compute(const DenseBase<Derived>& std_matrix);

		template<typename DerivedA, typename DerivedB>
		static void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Matrix standardize(const DenseBase<Derived>& x);

		template<typename Derived>
		decltype(auto) compute_holder(const DenseBase<Derived>& std_matrix);

		Index rows() const;
		Index cols() const;

		const MatrixType& std_matrix() const;
		MatrixType& const_cast_std_matrix() const;

		const Decomposition<Matrix>& decomposition() const;
		Decomposition<Matrix>& const_cast_decomposition() const;
	protected:
		ComputeHolderBase() = default;

		MatrixType m_std_matrix;
		Decomposition<Matrix> m_decomposition = Decomposition<Matrix>(m_std_matrix.rows(), m_std_matrix.cols());

		bool m_is_initialized = false;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(Index rows, Index cols)
		: m_std_matrix(rows, cols)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<Derived>& std_matrix)
		: m_std_matrix(std_matrix.derived())
		, m_decomposition(m_std_matrix)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<Derived>& std_matrix)
		: m_std_matrix(std_matrix.derived())
		, m_decomposition(m_std_matrix)
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename Derived>
	void ComputeHolderBase<DerivedType>::compute(const DenseBase<Derived>& std_matrix)
	{
		m_std_matrix = std_matrix.derived();
		m_decomposition.compute(m_std_matrix);
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
	decltype(auto) ComputeHolderBase<DerivedType>::compute_holder(const DenseBase<Derived>& std_matrix)
	{
		return Base::derived().compute_holder(std_matrix);
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::rows() const
	{
		assert(m_is_initialized);
		return m_std_matrix.rows();
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::cols() const
	{
		assert(m_is_initialized);
		return m_std_matrix.cols();
	}

	template<typename DerivedType>
	const typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::std_matrix() const
	{
		assert(m_is_initialized);
		return m_std_matrix;
	}

	template<typename DerivedType>
	typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::const_cast_std_matrix() const
	{
		assert(m_is_initialized);
		return const_cast<MatrixType&>(m_std_matrix);
	}

	template<typename DerivedType>
	const Decomposition<Matrix>& ComputeHolderBase<DerivedType>::decomposition() const
	{
		assert(m_is_initialized);
		return m_decomposition;
	}

	template<typename DerivedType>
	Decomposition<Matrix>& ComputeHolderBase<DerivedType>::const_cast_decomposition() const
	{
		assert(m_is_initialized);
		return m_decomposition;
	}
} // namespace kanalysis::stats
