#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	struct WeightFunction;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class KruskalWeight : public KruskalBase<KruskalWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<KruskalWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		KruskalWeight() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y, int threads);

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y);
	private:
		Vector m_standardized_y;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<KruskalWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& KruskalWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& y, int threads)
	{
		assert(y.rows() == Base::rows());
		WeightFunction::multiply_by_sqrt_weights(y, Base::compute_holder().sqrt_weights(), m_standardized_y);
		return Base::standardized_solve(m_standardized_y, threads);
	}

	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& KruskalWeight<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& y)
	{
		return solve(y, 1);
	}
} // namespace kanalysis::stats