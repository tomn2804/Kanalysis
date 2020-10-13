#pragma once

#include "config.h"

#include "stats/coefficient_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType>
	class Decomposition;

	///
	/// \brief A class for computing the beta coefficients.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class Coefficient : public CoefficientBase<Coefficient<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<Coefficient<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Coefficient() = default;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Coefficient<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	Coefficient<Decomposition<MatrixType>, RegressionFunctionType> coefficient(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A global factory function.
	///
	/// \param qr A \a Decomposition .
	/// \return A new \a Coefficient .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	Coefficient<Decomposition<MatrixType>, RegressionFunctionType> coefficient(const Decomposition<MatrixType>& qr)
	{
		return Coefficient<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
