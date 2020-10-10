#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	///
	/// \brief A CRTP base class for \a Coefficient and \a CoefficientWeight .
	///
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
	///
	/// \brief Find the beta coefficients of \a std_y regressing on the underlying qr.
	///
	/// \param std_y A vector with standardized values.
	/// \return A vector of beta coefficients.
	///
	template<typename DerivedType>
	template<typename Derived>
	const Vector& CoefficientBase<DerivedType>::solve(const VectorBase<Derived>& std_y) const
	{
		assert(std_y.rows() == Base::rows());
		RegressionFunctionType::coefficients(Base::qr().householder_qr(), std_y, m_results);
		return m_results;
	}
} // namespace kanalysis::stats
