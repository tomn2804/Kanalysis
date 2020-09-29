#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	class CoefficientBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::RegressionFunctionType;
		using Base::Base;
	public:
		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y);
	protected:
		CoefficientBase() = default;
		Vector m_results;
	private:
		friend class Base;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	const Vector& CoefficientBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		assert(y.rows() == Base::rows());
		RegressionFunctionType::coefficients(Base::decomposition(), y, m_results);
		return m_results;
	}
} // namespace kanalysis::stats