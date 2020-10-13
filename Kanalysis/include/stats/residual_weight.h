#pragma once

#include "config.h"

#include "stats/residual_base.h"
#include "stats/weight_function.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	///
	/// \brief A class for computing the weighted residuals.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class ResidualWeight : public ResidualBase<ResidualWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = ResidualBase<ResidualWeight<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		ResidualWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<ResidualWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	ResidualWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> residual(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& ResidualWeight<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		Vector& results = Base::std_solve(std_y);;
		WeightFunction::divide_by_sqrt_weights(results, Base::qr().sqrt_weights(), results);
		return results;
	}

	template<typename MatrixType, typename RegressionFunctionType>
	ResidualWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> residual(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return ResidualWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
