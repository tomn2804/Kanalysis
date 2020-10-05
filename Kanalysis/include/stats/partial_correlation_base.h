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
		using Base::Base;
	public:
		template<typename Derived>
		Scalar solve(const VectorBase<Derived>& std_y) const;

		template<typename DerivedA, typename DerivedB>
		Scalar solve(const VectorBase<DerivedA>& std_y, const VectorBase<DerivedB>& std_x) const;

		const auto& correlation() const;
		const auto& residual() const;

		const Vector& recent_residauls_x() const;
		const Vector& recent_residuals_y() const;
	protected:
		PartialCorrelationBase() = default;

		Correlation<const ComputeHolderDecayType&, RegressionFunctionType> m_correlation = Correlation<const ComputeHolderDecayType&, RegressionFunctionType>(Base::decomposition());

		mutable Vector m_residuals_x = Vector::Constant(Base::rows(), 0);;
		mutable Vector m_residuals_y = Vector::Constant(Base::rows(), 0);;

		mutable Scalar m_results = 0;

		mutable bool m_is_x_initialized = false;
	private:
		friend class SolveHolderBase<DerivedType>;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	Scalar PartialCorrelationBase<DerivedType>::solve(const VectorBase<Derived>& std_y) const
	{
		assert(m_is_x_initialized);
		m_residuals_y = Base::derived().m_residual.solve(std_y);
		m_results = m_correlation.cor(m_residuals_x, m_residuals_y);
		return m_results;
	}

	template<typename DerivedType>
	template<typename DerivedA, typename DerivedB>
	Scalar PartialCorrelationBase<DerivedType>::solve(const VectorBase<DerivedA>& std_y, const VectorBase<DerivedB>& std_x) const
	{
		m_residuals_x = Base::derived().m_residual.solve(std_x);
		m_is_x_initialized = true;
		return solve(std_y);
	}

	template<typename DerivedType>
	const auto& PartialCorrelationBase<DerivedType>::correlation() const
	{
		return m_correlation;
	}

	template<typename DerivedType>
	const auto& PartialCorrelationBase<DerivedType>::residual() const
	{
		return Base::derived().m_residual;
	}

	template<typename DerivedType>
	const Vector& PartialCorrelationBase<DerivedType>::recent_residauls_x() const
	{
		return m_residuals_x;
	}

	template<typename DerivedType>
	const Vector& PartialCorrelationBase<DerivedType>::recent_residuals_y() const
	{
		return m_residuals_y;
	}
} // namespace kanalysis::stats
