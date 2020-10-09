#pragma once

#include "include/config.h"

#include "include/stats/coefficient_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Coefficient : public CoefficientBase<Coefficient<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CoefficientBase<Coefficient<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Coefficient() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Coefficient<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	Coefficient<ComputeHolder<MatrixType>, RegressionFunctionType> coefficient(const ComputeHolder<MatrixType>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A global factory function.
	///
	/// \param decomposition A \a ComputeHolder .
	/// \return A new \a Coefficient .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	Coefficient<ComputeHolder<MatrixType>, RegressionFunctionType> coefficient(const ComputeHolder<MatrixType>& decomposition)
	{
		return Coefficient<ComputeHolder<MatrixType>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
