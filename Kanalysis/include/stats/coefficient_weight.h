#pragma once

#include "include/config.h"

#include "include/stats/coefficient_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	///
	/// \brief A class for computing the weighted beta coefficients.
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	class CoefficientWeight : public CoefficientBase<CoefficientWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<CoefficientWeight<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		CoefficientWeight() = default;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<CoefficientWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	CoefficientWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> coefficient(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A global factory function.
	///
	/// \param qr A \a DecompositionWeight .
	/// \return A new \a Coefficient .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	CoefficientWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> coefficient(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return CoefficientWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
}
