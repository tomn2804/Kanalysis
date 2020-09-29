#pragma once

#include "include/config.h"

namespace kanalysis::stats
{
	// y = bX + e
	struct LinearRegressionFunction
	{
		// b
		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void coefficients(const Decomposition<DerivedA>& X, const VectorBase<DerivedB>& y, VectorBase<DerivedC>& out);

		// y^ = bX
		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void fitted_values(const MatrixBase<DerivedA>& X, const VectorBase<DerivedB>& coefficients, VectorBase<DerivedC>& out);

		// e = y - y^
		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void residuals(const VectorBase<DerivedA>& y, const VectorBase<DerivedB>& fitted_values, VectorBase<DerivedC>& out);
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void LinearRegressionFunction::coefficients(const Decomposition<DerivedA>& X, const VectorBase<DerivedB>& y, VectorBase<DerivedC>& out)
	{
		assert(X.rows() == y.rows());
		out.derived().noalias() = X.solve(y);
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void LinearRegressionFunction::fitted_values(const MatrixBase<DerivedA>& X, const VectorBase<DerivedB>& coefficients, VectorBase<DerivedC>& out)
	{
		assert(X.cols() == coefficients.rows());
		out.derived().noalias() = X.derived() * coefficients.derived();
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void LinearRegressionFunction::residuals(const VectorBase<DerivedA>& y, const VectorBase<DerivedB>& fitted_values, VectorBase<DerivedC>& out)
	{
		assert(y.rows() == fitted_values.rows());
		out.derived().noalias() = y.derived() - fitted_values.derived();
	}
} // namespace kanalysis::stats
