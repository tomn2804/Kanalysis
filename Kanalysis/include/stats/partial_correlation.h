#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class PartialCorrelation : public PartialCorrelationBase<PartialCorrelation<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelation<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelation<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats