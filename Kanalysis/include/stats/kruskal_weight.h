#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight;

	///
	/// \brief A class for computing weighted Kruskal's relative importance.
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	class KruskalWeight : public KruskalBase<KruskalWeight<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<KruskalWeight<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		KruskalWeight() = default;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<KruskalWeight<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType>
	KruskalWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const DecompositionWeight<MatrixType, Array>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	KruskalWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType> kruskal(const DecompositionWeight<MatrixType, Array>& qr)
	{
		return KruskalWeight<DecompositionWeight<MatrixType, Array>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
