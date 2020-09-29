#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValue;

	template<typename DerivedType>
	class ResidualBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
		using Base::Base;
	public:
		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y);

		const auto& fitted_value() const;
	protected:
		ResidualBase() = default;
		FittedValue<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValue<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());
		Vector m_results;
	private:
		friend class Base;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	const Vector& ResidualBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		assert(y.rows() == Base::rows());
		const Vector& fitted_values = m_fitted_value.solve(y);
		RegressionFunctionType::residuals(y, fitted_values, m_results);
		return m_results;
	}

	template<typename DerivedType>
	const auto& ResidualBase<DerivedType>::fitted_value() const
	{
		return m_fitted_value;
	}
} // namespace kanalysis::stats