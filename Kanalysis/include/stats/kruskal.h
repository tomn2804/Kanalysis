#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Kruskal : public KruskalBase<Kruskal<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<Kruskal<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Kruskal() = default;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y, int threads);

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& y);
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Kruskal<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& Kruskal<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& y, int threads)
	{
		assert(y.rows() == Base::rows());
		return Base::standardized_solve(y, threads);
	}

	template<typename ComputeHolderType, typename RegressionFunctionType>
	template<typename Derived>
	const Vector& Kruskal<ComputeHolderType, RegressionFunctionType>::solve(const VectorBase<Derived>& y)
	{
		return solve(y, 1);
	}
} // namespace kanalysis::stats