#pragma once

#include "include/config.h"

#include "include/stats/fitted_value_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValueWeight : public FittedValueBase<FittedValueWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValueWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValueWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValueWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	FittedValueWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> fitted_value(const ComputeHolderWeight<MatrixType, Array>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& FittedValueWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		Vector& results = Base::std_solve(std_y);
		WeightFunction::divide_by_sqrt_weights(results, Base::decomposition().sqrt_weights(), results);
		return results;
	}

	template<typename MatrixType, typename RegressionFunctionType>
	FittedValueWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> fitted_value(const ComputeHolderWeight<MatrixType, Array>& decomposition)
	{
		return FittedValueWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
