#pragma once

#include "include/config.h"

#include "include/stats/kruskal_base.h"

namespace kanalysis::stats
{
	struct LinearRegressionFunction;

	template<typename MatrixType>
	class Decomposition;

	///
	/// \brief A class for computing Kruskal's relative importance.
	///
	template<typename DecompositionType, typename RegressionFunctionType>
	class Kruskal : public KruskalBase<Kruskal<DecompositionType, RegressionFunctionType>>
	{
	protected:
		using Base = KruskalBase<Kruskal<DecompositionType, RegressionFunctionType>>;
	public:
		using Base::Base;
		Kruskal() = default;
	};

	template<typename DecompositionType_, typename RegressionFunctionType_>
	struct SolveHolderTraits<Kruskal<DecompositionType_, RegressionFunctionType_>>
	{
		using DecompositionType = DecompositionType_;
		using RegressionFunctionType = RegressionFunctionType_;
	};

	template<typename MatrixType, typename RegressionFunctionType = LinearRegressionFunction>
	Kruskal<Decomposition<MatrixType>, RegressionFunctionType> kruskal(const Decomposition<MatrixType>& qr);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType, typename RegressionFunctionType>
	Kruskal<Decomposition<MatrixType>, RegressionFunctionType> kruskal(const Decomposition<MatrixType>& qr)
	{
		return Kruskal<Decomposition<MatrixType>, RegressionFunctionType>(qr);
	}
} // namespace kanalysis::stats
