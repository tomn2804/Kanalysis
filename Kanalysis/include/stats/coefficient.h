#pragma once

#include "include/config.h"

#include "include/stats/coefficient_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Coefficient : public CoefficientBase<Coefficient<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<Coefficient<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Coefficient() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Coefficient<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats