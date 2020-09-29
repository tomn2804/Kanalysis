#pragma once

#include "include/config.h"

#include "include/stats/correlation_base.h"

namespace kanalysis::stats
{
	struct WeightFunction;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class CorrelationWeight : public CorrelationBase<CorrelationWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CorrelationBase<CorrelationWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		CorrelationWeight() = default;

		template<typename DerivedA, typename DerivedB>
		Scalar cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y);

		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& y);

		template<typename Derived>
		Scalar standardized_solve(const VectorBase<Derived>& standardized_y);
	private:
		Vector m_normalized_y;
		using Base::m_mean_deviations_x;
		using Base::m_mean_deviations_y;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<CorrelationWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename DerivedA, typename DerivedB>
	Scalar CorrelationWeight<ComputeHolderType, RegressionFunctionType>::cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y)
	{
		assert(x.rows() == y.rows());

		const Array& weights = Base::weights();

		Scalar sum_weights = weights.sum();
		Scalar mean_x = (x.derived().array() * weights).sum() / sum_weights;
		Scalar mean_y = (y.derived().array() * weights).sum() / sum_weights;

		m_mean_deviations_x = x.derived().array() - mean_x;
		m_mean_deviations_y = y.derived().array() - mean_y;

		Scalar cov_x_y = (m_mean_deviations_x * m_mean_deviations_y * weights).sum();

		Scalar var_x = (m_mean_deviations_x.square() * weights).sum();
		Scalar var_y = (m_mean_deviations_y.square() * weights).sum();

		Base::m_results = cov_x_y / std::sqrt(var_x * var_y);
		return Base::m_results;
	}

	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	Scalar CorrelationWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& y)
	{
		assert(y.rows() == Base::rows());
		return cor(Base::m_fitted_value.solve(y), y); // solve will converts y to standardized_y.
	}

	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	Scalar CorrelationWeight<ComputeHolderType, RegressionFunctionType>::standardized_solve(const VectorBase<Derived>& standardized_y)
	{
		assert(standardized_y.rows() == Base::rows());
		WeightFunction::divide_by_sqrt_weights(standardized_y, Base::sqrt_weights(), m_normalized_y);
		return cor(Base::m_fitted_value.standardized_solve(standardized_y), m_normalized_y);
	}
} // namespace kanalysis::stats