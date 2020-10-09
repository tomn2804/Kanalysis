#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight;

	///
	/// \brief A class for computing weighted Kruskal's relative importance.
	///
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
	KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const ComputeHolderWeight<MatrixType, Array>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const ComputeHolderWeight<MatrixType, Array>& decomposition)
	{
		return KruskalWeight<ComputeHolderWeight<MatrixType, Array>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
