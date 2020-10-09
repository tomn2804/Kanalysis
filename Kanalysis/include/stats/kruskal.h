#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType>
	class ComputeHolder;

	///
	/// \brief A class for computing Kruskal's relative importance.
	///
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
	Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType> kruskal(const ComputeHolder<MatrixType>& decomposition);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType> kruskal(const ComputeHolder<MatrixType>& decomposition)
	{
		return Kruskal<ComputeHolder<MatrixType>, RegressionFunctionType>(decomposition);
	}
} // namespace kanalysis::stats
