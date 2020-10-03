#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class Kruskal : public KruskalBase<Kruskal<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<Kruskal<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Kruskal() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Kruskal<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType> kruskal(const ComputeHolder<MatrixType>& compute_holder);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType> kruskal(const ComputeHolder<MatrixType>& compute_holder)
	{
		return Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType>(compute_holder);
	}
} // namespace kanalysis::stats
