#pragma once

#include "include/config.h"

#include "include/stats/fitted_value_base.h"

namespace kanalysis::stats
{
	struct WeightFunction;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValueWeight : public FittedValueBase<FittedValueWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValueWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValueWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& standardized_y);
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValueWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& FittedValueWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& standardized_y)
	{
		assert(standardized_y.rows() == Base::rows());
		Vector& results = const_cast<Vector&>(Base::solve(standardized_y));
		WeightFunction::divide_by_sqrt_weights(Base::sqrt_weights(), results);
		return results;
	}
} // namespace kanalysis::stats