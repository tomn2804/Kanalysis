#pragma once

#include "config.h"

#include "stats/correlation_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType>
	class Decomposition;

	template<typename DecompositionType, typename RegressionFunctionType>
	class FittedValue;

	///
	/// \brief A class for computing the correlation.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class Correlation : public CorrelationBase<Correlation<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = CorrelationBase<Correlation<DecompositionType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		Correlation() = default;

		template<typename DerivedA, typename DerivedB>
		Scalar cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y) const;

		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& std_y) const;
	protected:
		FittedValue<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValue<const ComputeHolderDecayType&, RegressionFunctionType>(Base::qr());
	private:
		using Base::m_mean_deviations_x;
		using Base::m_mean_deviations_y;
	private:
		friend class CorrelationBase<Correlation<DecompositionType, RegressionFunctionType>>;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Correlation<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	Correlation<Decomposition<MatrixType>, RegressionFunctionType> correlation(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename DerivedA, typename DerivedB>
	Scalar Correlation<DecompositionType, RegressionFunctionType>::cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y) const
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

	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	Scalar Correlation<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		return cor(m_fitted_value.solve(std_y), std_y);
	}

	template<typename MatrixType, typename RegressionFunctionType>
	Correlation<Decomposition<MatrixType>, RegressionFunctionType> correlation(const Decomposition<MatrixType>& qr)
	{
		return Correlation<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
