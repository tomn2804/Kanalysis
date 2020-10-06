#pragma once

#include "include/config.h"

#define INTERCEPT_COL_INDEX 0
#define INTERCEPT_COL_VALUE 1

namespace kanalysis::utils
{
	template<typename Derived>
	Matrix as_model_matrix(const DenseBase<Derived>& x);
} // namespace kanalysis::utils

namespace kanalysis::utils
{
	///
	/// \brief Build a model matrix, also known as design matrix, for a simple linear regression or a multiple regression.
	///
	/// \param x A non-model matrix.
	/// \return A model matrix.
	///
	template<typename Derived>
	Matrix as_model_matrix(const DenseBase<Derived>& x)
	{
		Matrix model_matrix(x.rows(), x.cols() + 1);
		model_matrix.col(INTERCEPT_COL_INDEX).fill(INTERCEPT_COL_VALUE);
		model_matrix.rightCols(model_matrix.cols() - 1) = x.derived();
		return model_matrix;
	}
} // namespace kanalysis::utils
