#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	template<typename ComputeHolderType, typename RegressionFunctionType>
	class KruskalWeight : public KruskalBase<KruskalWeight<ComputeHolderType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<KruskalWeight<ComputeHolderType, RegressionFunctionType>>;
	public:
		using Base::Base;
		KruskalWeight() = default;
	};

	template<typename ComputeHolderType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<KruskalWeight<ComputeHolderType_, RegressionFunctionType_>>
	{
		using ComputeHolderType = ComputeHolderType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const ComputeHolderWeight<MatrixType, Array>& compute_holder);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const ComputeHolderWeight<MatrixType, Array>& compute_holder)
	{
		return KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(compute_holder);
	}
} // namespace kanalysis::stats
