#pragma once

#include "include/config.h"

#define MODEL_MATRIX_INTERCEPT_COL_INDEX 0
#define MODEL_MATRIX_INTERCEPT_COL_VALUE 1

namespace kanalysis::utils
{
	template<typename ForwardIterator, typename Int>
	bool contains(ForwardIterator first, ForwardIterator last, Int value);

	template<typename Derived>
	Matrix as_model_matrix(const DenseBase<Derived>& x);
} // namespace kanalysis::utils

namespace kanalysis::utils
{
	template<typename ForwardIterator, typename Int>
	bool contains(ForwardIterator first, ForwardIterator last, Int value)
	{
		assert(std::distance(first, last) >= 0);

		for (; first != last; ++first)
		{
			if (*first == value) return true;
		}
		return false;
	}

	template<typename Derived>
	Matrix as_model_matrix(const DenseBase<Derived>& x)
	{
		Matrix model_matrix(x.rows(), x.cols() + 1);
		model_matrix.col(MODEL_MATRIX_INTERCEPT_COL_INDEX).fill(MODEL_MATRIX_INTERCEPT_COL_VALUE);
		model_matrix.rightCols(model_matrix.cols() - 1) = x.derived();
		return model_matrix;
	}
} // namespace kanalysis::utils
