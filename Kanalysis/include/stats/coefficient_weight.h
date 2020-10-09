#pragma once

#include "include/config.h"

#include "include/stats/coefficient_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	///
	/// \brief A class for computing the weighted beta coefficients.
	///
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class CoefficientWeight : public CoefficientBase<CoefficientWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<CoefficientWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		CoefficientWeight() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<CoefficientWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	CoefficientWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> coefficient(const ComputeHolderWeight<MatrixType, Array>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A global factory function.
	///
	/// \param decomposition A \a ComputeHolderWeight .
	/// \return A new \a Coefficient .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	CoefficientWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> coefficient(const ComputeHolderWeight<MatrixType, Array>& decomposition)
	{
		return CoefficientWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(decomposition);
	}
}
