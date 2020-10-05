#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct ComputeHolderTraits;

	template<typename DerivedType>
	class CorrelationBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using Base::Base;
	public:
		const auto& fitted_value() const;

		const Array& mean_deviations_x() const;
		const Array& mean_deviations_y() const;
	protected:
		CorrelationBase() = default;

		mutable Array m_mean_deviations_x = Array::Constant(Base::rows(), 0);
		mutable Array m_mean_deviations_y = Array::Constant(Base::rows(), 0);

		mutable Scalar m_results = 0;
	private:
		friend class SolveHolderBase<DerivedType>;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	const auto& CorrelationBase<DerivedType>::fitted_value() const
	{
		return Base::derived().m_fitted_value;
	}

	template<typename DerivedType>
	const Array& CorrelationBase<DerivedType>::mean_deviations_x() const
	{
		return m_mean_deviations_x;
	}

	template<typename DerivedType>
	const Array& CorrelationBase<DerivedType>::mean_deviations_y() const
	{
		return m_mean_deviations_y;
	}
} // namespace kanalysis::stats
