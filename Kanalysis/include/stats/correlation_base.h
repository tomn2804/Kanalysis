#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct ComputeHolderTraits;

	template<typename DerivedType>
	class CorrelationBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using FittedValueTemplateType = typename ComputeHolderTraits<ComputeHolderDecayType>::template FittedValueTemplateType<ComputeHolderType_, RegressionFunctionType_>;

		using Base::Base;
	public:
		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& y);

		const auto& fitted_value() const;

		const Array& mean_deviations_x() const;
		const Array& mean_deviations_y() const;
	protected:
		CorrelationBase() = default;

		FittedValueTemplateType<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValueTemplateType<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());

		Array m_mean_deviations_x;
		Array m_mean_deviations_y;

		Scalar m_results;
	private:
		friend class Base;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	Scalar CorrelationBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		assert(standardized_y.rows() == Base::rows());
		return Base::derived().cor(m_fitted_value.standardized_solve(standardized_y), standardized_y);
	}

	template<typename DerivedType>
	const auto& CorrelationBase<DerivedType>::fitted_value() const
	{
		return m_fitted_value;
	}

	template<typename DerivedType>
	const Array& CorrelationBase<DerivedType>::mean_deviations_x() const
	{
		return m_mean_deviations_x;
	}

	template<typename DerivedType>
	const Array& CorrelationBase<DerivedType>::mean_deviations_y() const
	{
		return m_mean_deviations_y;
	}
} // namespace kanalysis::stats