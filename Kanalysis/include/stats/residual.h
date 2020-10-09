#pragma once

#include "include/config.h"

#include "include/stats/residual_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	///
	/// \brief A class for computing the residuals.
	///
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Residual : public ResidualBase<Residual<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<Residual<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Residual() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Residual<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	Residual<ComputeHolder<MatrixType>, RegressionFunctionType> residual(const ComputeHolder<MatrixType>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& Residual<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		return Base::std_solve(std_y);
	}

	template<typename MatrixType, typename RegressionFunctionType>
	Residual<ComputeHolder<MatrixType>, RegressionFunctionType> residual(const ComputeHolder<MatrixType>& decomposition)
	{
		return Residual<ComputeHolder<MatrixType>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
