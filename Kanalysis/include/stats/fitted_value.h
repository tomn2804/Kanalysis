#pragma once

#include "include/config.h"

#include "include/stats/fitted_value_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	///
	/// \brief A class for computing the fitted values.
	///
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValue : public FittedValueBase<FittedValue<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = FittedValueBase<FittedValue<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		FittedValue() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<FittedValue<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	FittedValue<ComputeHolder<MatrixType>, RegressionFunctionType> fitted_value(const ComputeHolder<MatrixType>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \overload Vector& FittedValueBase<DerivedType>::std_solve(const VectorBase<Derived>& std_y) const
	///
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& FittedValue<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		return Base::std_solve(std_y);
	}

	///
	/// \brief A global factory function.
	///
	/// \param decomposition A \a ComputeHolder .
	/// \return A new \a FittedValue .
	///
	template<typename MatrixType, typename RegressionFunctionType>
	FittedValue<ComputeHolder<MatrixType>, RegressionFunctionType> fitted_value(const ComputeHolder<MatrixType>& decomposition)
	{
		return FittedValue<ComputeHolder<MatrixType>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
