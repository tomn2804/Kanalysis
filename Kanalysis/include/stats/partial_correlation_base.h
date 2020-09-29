#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct ComputeHolderTraits;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Correlation;

	template<typename DerivedType>
	class PartialCorrelationBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using ResidualTemplateType = typename ComputeHolderTraits<ComputeHolderDecayType>::template ResidualTemplateType<ComputeHolderType_, RegressionFunctionType_>;

		using Base::Base;
	public:
		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& y);

		template<typename DerivedA, typename DerivedB>
		Scalar solve(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y);

		template<typename Derived>
		Scalar standardized_solve(const VectorBase<Derived>& standardized_y);

		template<typename DerivedA, typename DerivedB>
		Scalar standardized_solve(const VectorBase<DerivedA>& standardized_x, const VectorBase<DerivedB>& standardized_y);

		const auto& correlation() const;
		const auto& residual() const;

		const Vector& residuals_x() const;
		const Vector& residuals_y() const;
	protected:
		PartialCorrelationBase() = default;

		Correlation<const ComputeHolderDecayType&, RegressionFunctionType> m_correlation = Correlation<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());
		ResidualTemplateType<const ComputeHolderDecayType&, RegressionFunctionType> m_residual = ResidualTemplateType<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());

		Vector m_residuals_x;
		Vector m_residuals_y;

		Scalar m_results;
	private:
		friend class Base;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	Scalar PartialCorrelationBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		assert(y.rows() == Base::rows());
		assert(y.rows() == m_residuals_y.rows());
		m_residuals_x = m_residual.solve(y);
		m_results = m_correlation.cor(m_residuals_x, m_residuals_y);
		return m_results;
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	Scalar PartialCorrelationBase<DerivedType>::solve(const VectorBase<DerivedA>& x, const VectorBase<DerivedB>& y)
	{
		assert(x.rows() == Base::rows());
		assert(y.rows() == Base::rows());
		m_residuals_y = m_residual.solve(y);
		return solve(x);
	}

	template<typename DerivedType>
	template<typename Derived>
	Scalar PartialCorrelationBase<DerivedType>::standardized_solve(const VectorBase<Derived>& standardized_y)
	{
		assert(standardized_y.rows() == Base::rows());
		assert(standardized_y.rows() == m_residuals_y.rows());
		m_residuals_x = m_residual.standardized_solve(standardized_y);
		m_results = m_correlation.cor(m_residuals_x, m_residuals_y);
		return m_results;
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	Scalar PartialCorrelationBase<DerivedType>::standardized_solve(const VectorBase<DerivedA>& standardized_x, const VectorBase<DerivedB>& standardized_y)
	{
		assert(standardized_x.rows() == Base::rows());
		assert(standardized_y.rows() == Base::rows());
		m_residuals_y = m_residual.standardized_solve(standardized_y);
		return standardized_solve(standardized_x);
	}

	template<typename DerivedType>
	const auto& PartialCorrelationBase<DerivedType>::correlation() const
	{
		return m_correlation;
	}

	template<typename DerivedType>
	const auto& PartialCorrelationBase<DerivedType>::residual() const
	{
		return m_residual;
	}

	template<typename DerivedType>
	const Vector& PartialCorrelationBase<DerivedType>::residuals_x() const
	{
		return m_residuals_x;
	}

	template<typename DerivedType>
	const Vector& PartialCorrelationBase<DerivedType>::residuals_y() const
	{
		return m_residuals_y;
	}
} // namespace kanalysis::stats