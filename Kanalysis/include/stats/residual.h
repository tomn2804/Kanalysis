#pragma once

#include "include/config.h"

#include "include/stats/residual_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class Decomposition;

	///
	/// \brief A class for computing the residuals.
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	class Residual : public ResidualBase<Residual<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<Residual<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Residual() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Residual<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	Residual<Decomposition<MatrixType>, RegressionFunctionType> residual(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& Residual<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		return Base::std_solve(std_y);
	}

	template<typename MatrixType, typename RegressionFunctionType>
	Residual<Decomposition<MatrixType>, RegressionFunctionType> residual(const Decomposition<MatrixType>& qr)
	{
		return Residual<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
