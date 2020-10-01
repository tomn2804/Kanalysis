#pragma once

#include "include/config.h"

#include "include/stats/correlation_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class FittedValue;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Correlation : public CorrelationBase<Correlation<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = CorrelationBase<Correlation<ComputeHolderType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		Correlation() = default;

		template<typename DerivedA, typename DerivedB>
		Scalar cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y);

		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& std_y);
	protected:
		FittedValue<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValue<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());
	private:
		using Base::m_mean_deviations_x;
		using Base::m_mean_deviations_y;
	private:
		friend class Base;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Correlation<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	Correlation<ComputeHolder<MatrixType>, RegressionFunctionType> correlation(const ComputeHolder<MatrixType>& compute_holder);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename DerivedA, typename DerivedB>
	Scalar Correlation<ComputeHolderType, RegressionFunctionType>::cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y)
	{
		assert(x.rows() == y.rows());

		Scalar mean_x = x.derived().mean();
		Scalar mean_y = y.derived().mean();

		m_mean_deviations_x = x.derived().array() - mean_x;
		m_mean_deviations_y = y.derived().array() - mean_y;

		Scalar cov_x_y = (m_mean_deviations_x * m_mean_deviations_y).sum();

		Scalar var_x = m_mean_deviations_x.square().sum();
		Scalar var_y = m_mean_deviations_y.square().sum();

		Base::m_results = cov_x_y / std::sqrt(var_x * var_y);
		return Base::m_results;
	}

	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	Scalar Correlation<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y)
	{
		return cor(m_fitted_value.solve(std_y), std_y);
	}

	template<typename MatrixType, typename RegressionFunctionType>
	Correlation<ComputeHolder<MatrixType>, RegressionFunctionType> correlation(const ComputeHolder<MatrixType>& compute_holder)
	{
		return Correlation<ComputeHolder<MatrixType>, RegressionFunctionType>(compute_holder);
	}
} // namespace kanalysis::stats
