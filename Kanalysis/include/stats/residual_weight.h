#pragma once

#include "include/config.h"

#include "include/stats/residual_base.h"

namespace kanalysis::stats
{
	struct WeightFunction;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class ResidualWeight : public ResidualBase<ResidualWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<ResidualWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		ResidualWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& standardized_y);
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<ResidualWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& ResidualWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& standardized_y)
	{
		assert(standardized_y.rows() == Base::rows());
		Vector& results = const_cast<Vector&>(Base::standardized_solve(standardized_y));
		WeightFunction::divide_by_sqrt_weights(Base::sqrt_weights(), results);
		return results;
	}
} // namespace kanalysis::stats