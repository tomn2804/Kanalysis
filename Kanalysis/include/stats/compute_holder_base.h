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
		ComputeHolderBase(const DenseBase<Derived>& standardized_matrix);

		template<typename Derived>
		ComputeHolderBase(DenseBase<Derived>& standardized_matrix);

		template<typename Derived>
		void compute(const DenseBase<Derived>& standardized_matrix);

		Index rows() const;
		Index cols() const;

		const MatrixType& standardized_matrix() const;
		MatrixType& const_cast_standardized_matrix() const;

		const Decomposition<Matrix>& decomposition() const;
		Decomposition<Matrix>& const_cast_decomposition() const;
	protected:
		ComputeHolderBase() = default;

		MatrixType m_standardized_matrix;
		Decomposition<Matrix> m_decomposition = Decomposition<Matrix>(m_standardized_matrix.rows(), m_standardized_matrix.cols());

		Array m_weights;
		Array m_sqrt_weights = m_weights.array().sqrt();

		bool m_is_initialized = false;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(Index rows, Index cols)
		: m_standardized_matrix(rows, cols)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<Derived>& matrix)
		: m_standardized_matrix(matrix.derived())
		, m_decomposition(m_standardized_matrix)
		, m_weights()
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename Derived>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<Derived>& matrix)
		: m_standardized_matrix(matrix.derived())
		, m_decomposition(m_standardized_matrix)
		, m_weights()
		, m_is_initialized(true)
	{}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights)
		: m_standardized_matrix(matrix.derived())
		, m_weights(weights.derived())
		, m_is_initialized(true)
	{
		// Derived class needs to call standardized_compute() to finish off the initialization.
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights)
		: m_standardized_matrix(matrix.derived())
		, m_weights(weights.derived())
		, m_is_initialized(true)
	{
		// Derived class needs to call standardized_compute() to finish off the initialization.
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: ComputeHolderBase(matrix, weights)
	{}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderBase<DerivedType>::ComputeHolderBase(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: ComputeHolderBase(matrix, weights)
	{}

	template<typename DerivedType>
	template<typename Derived>
	void ComputeHolderBase<DerivedType>::compute(const DenseBase<Derived>& matrix)
	{
		Base::derived().compute(matrix);
	}

	template<typename DerivedType>
	template<typename Derived>
	void ComputeHolderBase<DerivedType>::standardized_compute(const DenseBase<Derived>& standardized_matrix)
	{
		m_standardized_matrix = standardized_matrix.derived();
		m_decomposition.compute(m_standardized_matrix);
		m_is_initialized = true;
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::rows() const
	{
		assert(m_is_initialized);
		return m_standardized_matrix.rows();
	}

	template<typename DerivedType>
	Index ComputeHolderBase<DerivedType>::cols() const
	{
		assert(m_is_initialized);
		return m_standardized_matrix.cols();
	}

	template<typename DerivedType>
	const typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::standardized_matrix() const
	{
		assert(m_is_initialized);
		return m_standardized_matrix;
	}

	template<typename DerivedType>
	typename ComputeHolderBase<DerivedType>::MatrixType& ComputeHolderBase<DerivedType>::const_cast_standardized_matrix() const
	{
		assert(m_is_initialized);
		return const_cast<MatrixType&>(m_standardized_matrix);
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
		return const_cast<Decomposition<Matrix>&>(m_decomposition);
	}

	template<typename DerivedType>
	const typename ComputeHolderBase<DerivedType>::ArrayType& ComputeHolderBase<DerivedType>::weights() const
	{
		assert(m_is_initialized);
		return m_weights;
	}

	template<typename DerivedType>
	typename ComputeHolderBase<DerivedType>::ArrayType& ComputeHolderBase<DerivedType>::const_cast_weights() const
	{
		assert(m_is_initialized);
		return const_cast<ArrayType&>(m_weights);
	}

	template<typename DerivedType>
	const Array& ComputeHolderBase<DerivedType>::sqrt_weights() const
	{
		assert(m_is_initialized);
		return m_sqrt_weights;
	}

	template<typename DerivedType>
	Array& ComputeHolderBase<DerivedType>::const_cast_sqrt_weights() const
	{
		assert(m_is_initialized);
		return const_cast<Array&>(m_sqrt_weights);
	}
} // namespace kanalysis::stats