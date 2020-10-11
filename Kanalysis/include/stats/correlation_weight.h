#pragma once

#include "include/config.h"

#include "include/stats/correlation_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	template<typename DecompositionType, typename RegressionFunctionType>
	class FittedValueWeight;

	///
	/// \brief A class for computing the weighted residuals.
	///
	template<typename DecompositionType, typename RegressionFunctionType = LinearRegressionFunction>
	class CorrelationWeight : public CorrelationBase<CorrelationWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = CorrelationBase<CorrelationWeight<DecompositionType, RegressionFunctionType>>;
		using typename Base::ComputeHolderDecayType;
	public:
		using Base::Base;
		CorrelationWeight() = default;

		template<typename DerivedA, typename DerivedB>
		Scalar cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y) const;

		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& std_y) const;
	protected:
		FittedValueWeight<const ComputeHolderDecayType&, RegressionFunctionType> m_fitted_value = FittedValueWeight<const ComputeHolderDecayType&, RegressionFunctionType>(Base::qr());
	private:
		using Base::m_mean_deviations_x;
		using Base::m_mean_deviations_y;

		mutable Vector m_normalized_y = Vector::Constant(Base::rows(), 0);
	private:
		friend class CorrelationBase<CorrelationWeight<DecompositionType, RegressionFunctionType>>;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<CorrelationWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	CorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> correlation(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename DerivedA, typename DerivedB>
	Scalar CorrelationWeight<DecompositionType, RegressionFunctionType>::cor(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y) const
	{
		assert(x.rows() == y.rows());

		const Array& weights = Base::qr().weights();

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

	template<typename DecompositionType, typename RegressionFunctionType>
	template<typename Derived>
	Scalar CorrelationWeight<DecompositionType, RegressionFunctionType>::solve(const VectorBase<Derived>& std_y) const
	{
		WeightFunction::divide_by_sqrt_weights(std_y, Base::qr().sqrt_weights(), m_normalized_y);
		return cor(m_fitted_value.solve(std_y), m_normalized_y);
	}

	template<typename MatrixType, typename RegressionFunctionType>
	CorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> correlation(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return CorrelationWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
