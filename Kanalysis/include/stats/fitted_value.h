#pragma once

#include "config.h"

#include "stats/fitted_value_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType>
	class Decomposition;

	///
	/// \brief A class for computing the fitted values.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class FittedValue : public FittedValueBase<FittedValue<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValue<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValue() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValue<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	FittedValue<Decomposition<MatrixType>, RegressionFunctionType> fitted_value(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \overload Vector& FittedValueBase<DerivedType>::std_solve(const VectorBase<Derived>& std_y) const
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& FittedValue<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		return Base::std_solve(std_y);
	}

	///
	/// \brief A global factory function.
	///
	/// \param qr A \a Decomposition .
	/// \return A new \a FittedValue .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	FittedValue<Decomposition<MatrixType>, RegressionFunctionType> fitted_value(const Decomposition<MatrixType>& qr)
	{
		return FittedValue<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
