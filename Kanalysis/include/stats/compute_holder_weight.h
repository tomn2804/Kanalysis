#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight : public ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>
	{
	protected:
		using Base = ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>;
	public:
		using Base::Base;
		ComputeHolderWeight() = default;

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(DenseBase<DerivedA>& std_matrix, DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		ComputeHolderWeight(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		ComputeHolderWeight(DenseBase<DerivedA>& std_matrix, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB>
		void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const;

		template<typename Derived>
		Matrix standardize(const DenseBase<Derived>& x) const;

		template<typename Derived>
		ComputeHolderWeight<Derived, ArrayType> compute_holder(const DenseBase<Derived>& std_matrix) const;

		const ArrayType& weights() const;
		ArrayType& const_cast_weights() const;

		const Array& sqrt_weights() const;
		Array& const_cast_sqrt_weights() const;
	private:
		ArrayType m_weights;
		Array m_sqrt_weights = WeightFunction::sqrt_weights(m_weights);
	};

	template<typename MatrixType_, typename ArrayType_>
	struct ComputeHolderTraits<ComputeHolderWeight<MatrixType_, ArrayType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = ArrayType_;
	};

	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<DerivedA, DerivedB> compute_holder(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights)
		: Base(std_matrix)
		, m_weights(weights.derived())
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& std_matrix, DenseBase<DerivedB>& weights)
		: Base(std_matrix)
		, m_weights(weights.derived())
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights)
		: Base(std_matrix)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& std_matrix, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights)
		: Base(std_matrix)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	void ComputeHolderWeight<MatrixType, ArrayType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const
	{
		WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights, out);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	Matrix ComputeHolderWeight<MatrixType, ArrayType>::standardize(const DenseBase<Derived>& x) const
	{
		return WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	ComputeHolderWeight<Derived, ArrayType> ComputeHolderWeight<MatrixType, ArrayType>::compute_holder(const DenseBase<Derived>& std_matrix) const
	{
		return ComputeHolderWeight<Derived, ArrayType>(std_matrix, m_weights, m_sqrt_weights);
	}

	template<typename MatrixType, typename ArrayType>
	const ArrayType& ComputeHolderWeight<MatrixType, ArrayType>::weights() const
	{
		return m_weights;
	}

	template<typename MatrixType, typename ArrayType>
	ArrayType& ComputeHolderWeight<MatrixType, ArrayType>::const_cast_weights() const
	{
		return const_cast<ArrayType&>(m_weights);
	}

	template<typename MatrixType, typename ArrayType>
	const Array& ComputeHolderWeight<MatrixType, ArrayType>::sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	template<typename MatrixType, typename ArrayType>
	Array& ComputeHolderWeight<MatrixType, ArrayType>::const_cast_sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<DerivedA, DerivedB> compute_holder(const DenseBase<DerivedA>& std_matrix, const DenseBase<DerivedB>& weights)
	{
		return ComputeHolderWeight<DerivedA, DerivedB>(std_matrix, weights);
	}
} // namespace kanalysis::stats
