#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class PartialCorrelationWeight : public PartialCorrelationBase<PartialCorrelationWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelationWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelationWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats