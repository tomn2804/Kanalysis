#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"

namespace kanalysis::stats
{
	struct WeightFunction;

	template<typename ComputeHolderTemplateType, typename RegressionFunctionType>
	class CoefficientWeight;

	template<typename ComputeHolderTemplateType, typename RegressionFunctionType>
	class FittedValueWeight;

	template<typename ComputeHolderTemplateType, typename RegressionFunctionType>
	class ResidualWeight;

	template<typename ComputeHolderTemplateType, typename RegressionFunctionType>
	class CorrelationWeight;

	template<typename ComputeHolderTemplateType, typename RegressionFunctionType>
	class PartialCorrelationWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class KruskalWeight;

	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight : public ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>
	{
	protected:
		using Base = ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>;
	public:
		using Base::Base;
		ComputeHolderWeight() = default;

		template<typename Derived>
		ComputeHolderWeight(const DenseBase<Derived>& matrix);

		template<typename Derived>
		ComputeHolderWeight(DenseBase<Derived>& matrix);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights, bool is_standardized_matrix);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights, bool is_standardized_matrix);

		template<typename Derived>
		void compute(const DenseBase<Derived>& matrix);

		const ArrayType& weights() const;
		ArrayType& const_cast_weights() const;

		const Array& sqrt_weights() const;
		Array& const_cast_sqrt_weights() const;
	};

	template<typename MatrixType_, typename ArrayType_>
	struct ComputeHolderTraits<ComputeHolderWeight<MatrixType_, ArrayType_>>
	{
		template<typename MatrixType, typename ArrayType>
		using ComputeHolderTemplateType = ComputeHolderWeight<MatrixType, ArrayType>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using CoefficientTemplateType = CoefficientWeight<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using FittedValueTemplateType = FittedValueWeight<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using ResidualTemplateType = ResidualWeight<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using CorrelationTemplateType = CorrelationWeight<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using PartialCorrelationTemplateType = PartialCorrelationWeight<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using KruskalTemplateType = KruskalWeight<ComputeHolderType_, RegressionFunctionType_>;

		using MatrixType = MatrixType_;
		using ArrayType = ArrayType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<Derived>& matrix)
		: Base(matrix, Vector::Constant(matrix.rows(), 1))
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<Derived>& matrix)
		: Base(matrix, Vector::Constant(matrix.rows(), 1))
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights)
		: Base(matrix, weights)
	{
		compute(matrix);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights)
		: Base(matrix, weights)
	{
		compute(matrix);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: Base(matrix, weights)
	{
		if (is_standardized_matrix) Base::standardized_compute(matrix);
		else compute(matrix);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: Base(matrix, weights)
	{
		if (is_standardized_matrix) Base::standardized_compute(matrix);
		else compute(matrix);
	}

	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	void ComputeHolderWeight<MatrixType, ArrayType>::compute(const DenseBase<Derived>& matrix)
	{
		WeightFunction::multiply_by_sqrt_weights(matrix, Base::m_sqrt_weights, Base::m_standardized_matrix);
		Base::standardized_compute(Base::m_standardized_matrix);
	}
} // namespace kanalysis::stats