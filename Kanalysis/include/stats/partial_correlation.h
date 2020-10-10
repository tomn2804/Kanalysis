#pragma once

#include "include/config.h"

#include "include/stats/partial_correlation_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType>
	class Decomposition;

	template<typename DecompositionType, typename RegressionFunctionType>
	class Residual;

	///
	/// \brief A class for computing the partial correlation.
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	class PartialCorrelation : public PartialCorrelationBase<PartialCorrelation<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = PartialCorrelationBase<PartialCorrelation<DecompositionType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		PartialCorrelation() = default;
	protected:
		Residual<const ComputeHolderDecayType&, RegressionFunctionType> m_residual = Residual<const ComputeHolderDecayType&, RegressionFunctionType>(Base::qr());
	private:
		friend class PartialCorrelationBase<PartialCorrelation<DecompositionType, RegressionFunctionType>>;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<PartialCorrelation<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	PartialCorrelation<Decomposition<MatrixType>, RegressionFunctionType> partial_correlation(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	PartialCorrelation<Decomposition<MatrixType>, RegressionFunctionType> partial_correlation(const Decomposition<MatrixType>& qr)
	{
		return PartialCorrelation<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
