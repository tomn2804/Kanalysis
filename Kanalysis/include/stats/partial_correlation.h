#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Residual;

	///
	/// \brief A class for computing the partial correlation.
	///
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class PartialCorrelation : public PartialCorrelationBase<PartialCorrelation<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelation<ComputeHolderType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		PartialCorrelation() = default;
	protected:
		Residual<const ComputeHolderDecayType&, RegressionFunctionType> m_residual = Residual<const ComputeHolderDecayType&, RegressionFunctionType>(Base::decomposition());
	private:
		friend class PartialCorrelationBase<PartialCorrelation<ComputeHolderType, RegressionFunctionType>>;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelation<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelation<ComputeHolder<MatrixType>, RegressionFunctionType> partial_correlation(const ComputeHolder<MatrixType>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelation<ComputeHolder<MatrixType>, RegressionFunctionType> partial_correlation(const ComputeHolder<MatrixType>& decomposition)
	{
		return PartialCorrelation<ComputeHolder<MatrixType>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
