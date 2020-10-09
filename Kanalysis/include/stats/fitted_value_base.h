#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

#include "include/stats/coefficient.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Coefficient;

	///
	/// \brief A CRTP base class for \a FittedValue and \a FittedValueWeight .
	///
	template<typename DerivedType>
	class FittedValueBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
		using Base::Base;
	public:
		const auto& coefficient() const;
	protected:
		FittedValueBase() = default;

		template<typename Derived>
		Vector& std_solve(const VectorBase<Derived>& std_y) const;

		Coefficient<const ComputeHolderDecayType&, RegressionFunctionType> m_coefficient = Coefficient<const ComputeHolderDecayType&, RegressionFunctionType>(Base::decomposition());
		mutable Vector m_results = Vector::Constant(Base::rows(), 0);
	private:
		friend class SolveHolderBase<DerivedType>;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \return The \a Coefficient object used to solve for fitted values.
	///
	template<typename DerivedType>
	const auto& FittedValueBase<DerivedType>::coefficient() const
	{
		return m_coefficient;
	}

	///
	/// \brief Find the fitted values of \a std_y regressing on the underlying decomposition.
	///
	/// \param std_y A vector with standardized values.
	/// \return A vector of fitted values.
	///
	template<typename DerivedType>
	template<typename Derived>
	Vector& FittedValueBase<DerivedType>::std_solve(const VectorBase<Derived>& std_y) const
	{
		assert(std_y.rows() == Base::rows());
		const Vector& coefficients = m_coefficient.solve(std_y);
		RegressionFunctionType::fitted_values(Base::std_x(), coefficients, m_results);
		return m_results;
	}
} // namespace kanalysis::stats
