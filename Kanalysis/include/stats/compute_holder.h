#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolder : public ComputeHolderBase<ComputeHolder<MatrixType, ArrayType>>
	{
	protected:
		using Base = ComputeHolderBase<ComputeHolder<MatrixType, ArrayType>>;
	public:
		using Base::Base;
		ComputeHolder() = default;

		template<typename DerivedA, typename DerivedB>
		ComputeHolder(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolder(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolder(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights, bool is_standardized_matrix);

		template<typename DerivedA, typename DerivedB>
		ComputeHolder(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights, bool is_standardized_matrix);

		template<typename Derived>
		void compute(const DenseBase<Derived>& matrix);
	};

	template<typename MatrixType_, typename ArrayType_>
	struct ComputeHolderTraits<ComputeHolder<MatrixType_, ArrayType_>>
	{
		template<typename MatrixType, typename ArrayType>
		using ComputeHolderTemplateType = ComputeHolder<MatrixType, ArrayType>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using CoefficientTemplateType = Coefficient<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using FittedValueTemplateType = FittedValue<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using ResidualTemplateType = Residual<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using CorrelationTemplateType = Correlation<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using PartialCorrelationTemplateType = PartialCorrelation<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using KruskalTemplateType = Kruskal<ComputeHolderType_, RegressionFunctionType_>;

		using MatrixType = MatrixType_;
		using ArrayType = ArrayType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolder<MatrixType, ArrayType>::ComputeHolder(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights)
		: Base(matrix)
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolder<MatrixType, ArrayType>::ComputeHolder(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights)
		: Base(matrix)
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolder<MatrixType, ArrayType>::ComputeHolder(const DenseBase<DerivedA>& matrix, const DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: Base(matrix)
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolder<MatrixType, ArrayType>::ComputeHolder(DenseBase<DerivedA>& matrix, DenseBase<DerivedB>& weights, bool is_standardized_matrix)
		: Base(matrix)
	{}

	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	void ComputeHolder<MatrixType, ArrayType>::compute(const DenseBase<Derived>& matrix)
	{
		Base::standardized_compute(matrix);
	}
} // namespace kanalysis::stats