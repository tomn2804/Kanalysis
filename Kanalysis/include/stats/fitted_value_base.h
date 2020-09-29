#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Coefficient;

	template<typename DerivedType>
	class FittedValueBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
		using Base::Base;
	public:
		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y);

		const auto& coefficient() const;
	protected:
		FittedValueBase() = default;
		Coefficient<const ComputeHolderDecayType&, RegressionFunctionType> m_coefficient = Coefficient<const ComputeHolderDecayType&, RegressionFunctionType>(Base::compute_holder());
		Vector m_results;
	private:
		friend class Base;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	const Vector& FittedValueBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		assert(y.rows() == Base::rows());
		const Vector& coefficients = m_coefficient.solve(y);
		RegressionFunctionType::fitted_values(Base::standardized_matrix(), coefficients, m_results);
		return m_results;
	}

	template<typename DerivedType>
	const auto& FittedValueBase<DerivedType>::coefficient() const
	{
		return m_coefficient;
	}
} // namespace kanalysis::stats