#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class ResidualWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class PartialCorrelationWeight : public PartialCorrelationBase<PartialCorrelationWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelationWeight<ComputeHolderType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		PartialCorrelationWeight() = default;
	protected:
		ResidualWeight<const ComputeHolderDecayType&, RegressionFunctionType> m_residual = ResidualWeight<const ComputeHolderDecayType&, RegressionFunctionType>(Base::decomposition());
	private:
		friend class PartialCorrelationBase<PartialCorrelationWeight<ComputeHolderType, RegressionFunctionType>>;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelationWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelationWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> partial_correlation(const ComputeHolderWeight<MatrixType, Array>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelationWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> partial_correlation(const ComputeHolderWeight<MatrixType, Array>& decomposition)
	{
		return PartialCorrelationWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
