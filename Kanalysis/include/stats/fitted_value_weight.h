#pragma once

#include "config.h"

#include "stats/fitted_value_base.h"
#include "stats/weight_function.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	///
	/// \brief A class for computing the weighted fitted values.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class FittedValueWeight : public FittedValueBase<FittedValueWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValueWeight<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValueWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValueWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	FittedValueWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> fitted_value(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \overload Vector& FittedValueBase<DerivedType>::std_solve(const VectorBase<Derived>& std_y) const
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& FittedValueWeight<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		Vector& results = Base::std_solve(std_y);
		WeightFunction::divide_by_sqrt_weights(results, Base::qr().sqrt_weights(), results);
		return results;
	}

	///
	/// \brief A global factory function.
	///
	/// \param qr A \a DecompositionWeight .
	/// \return A new \a FittedValueWeight .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	FittedValueWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> fitted_value(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return FittedValueWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
