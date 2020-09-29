#pragma once

#include "include/config.h"

#include "include/stats/residual_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Residual : public ResidualBase<Residual<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<Residual<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Residual() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Residual<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats