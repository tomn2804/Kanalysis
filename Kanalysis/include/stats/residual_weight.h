#pragma once

#include "include/config.h"

#include "include/stats/residual_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class ResidualWeight : public ResidualBase<ResidualWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<ResidualWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		ResidualWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<ResidualWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	ResidualWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> residual(const ComputeHolderWeight<MatrixType, Array>& compute_holder);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& ResidualWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		Vector& results = Base::std_solve(std_y);;
		WeightFunction::divide_by_sqrt_weights(results, Base::decomposition().sqrt_weights(), results);
		return results;
	}

	template<typename MatrixType, typename RegressionFunctionType>
	ResidualWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> residual(const ComputeHolderWeight<MatrixType, Array>& compute_holder)
	{
		return ResidualWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(compute_holder);
	}
} // namespace kanalysis::stats
