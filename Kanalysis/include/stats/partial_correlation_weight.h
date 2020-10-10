#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	template<typename DecompositionType, typename RegressionFunctionType>
	class ResidualWeight;

	template<typename DecompositionType, typename RegressionFunctionType>
	class PartialCorrelationWeight : public PartialCorrelationBase<PartialCorrelationWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelationWeight<DecompositionType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		PartialCorrelationWeight() = default;
	protected:
		ResidualWeight<const ComputeHolderDecayType&, RegressionFunctionType> m_residual = ResidualWeight<const ComputeHolderDecayType&, RegressionFunctionType>(Base::qr());
	private:
		friend class PartialCorrelationBase<PartialCorrelationWeight<DecompositionType, RegressionFunctionType>>;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelationWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> partial_correlation(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> partial_correlation(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return PartialCorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
