#pragma once

#include "include/config.h"

#include "include/stats/fitted_value_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValue : public FittedValueBase<FittedValue<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValue<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValue() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValue<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats