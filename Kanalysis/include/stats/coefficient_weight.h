#pragma once

#include "include/config.h"

#include "include/stats/coefficient_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class CoefficientWeight : public CoefficientBase<CoefficientWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<CoefficientWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		CoefficientWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& standardized_y);
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<CoefficientWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& CoefficientWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& standardized_y)
	{
		return Base::solve(standardized_y);
	}
} // namespace kanalysis::stats