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
		const Vector& solve(const VectorBase<Derived>& std_y) const;
	protected:
		CoefficientBase() = default;
		mutable Vector m_results = Vector::Constant(Base::cols(), 0);
	private:
		friend class SolveHolderBase<DerivedType>;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	const Vector& CoefficientBase<DerivedType>::solve(const VectorBase<Derived>& std_y) const
	{
		assert(std_y.rows() == Base::rows());
		RegressionFunctionType::coefficients(Base::decomposition().householder_qr(), std_y, m_results);
		return m_results;
	}
} // namespace kanalysis::stats
