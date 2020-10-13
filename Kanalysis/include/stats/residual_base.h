#pragma once

#include "config.h"

#include "stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename DecompositionType, typename RegressionFunctionType>
	class FittedValue;

	///
	/// \brief A CRTP base class for \a Residual and \a ResidualWeight .
	///
	template<typename DerivedType>
	class ResidualBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
		using Base::Base;
	public:
		const auto& fitted_value() const;
	protected:
		ResidualBase() = default;

		template<typename Derived>
		Vector& std_solve(const VectorBase<Derived>& std_y) const;

		FittedValue<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValue<const ComputeHolderDecayType&, RegressionFunctionType>(Base::qr());
		mutable Vector m_results = Vector::Constant(Base::rows(), 0);
	private:
		friend class SolveHolderBase<DerivedType>;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	const auto& ResidualBase<DerivedType>::fitted_value() const
	{
		return m_fitted_value;
	}

	template<typename DerivedType>
	template<typename Derived>
	Vector& ResidualBase<DerivedType>::std_solve(const VectorBase<Derived>& std_y) const
	{
		assert(std_y.rows() == Base::rows());
		const Vector& fitted_values = m_fitted_value.solve(std_y);
		RegressionFunctionType::residuals(std_y, fitted_values, m_results);
		return m_results;
	}
} // namespace kanalysis::stats
