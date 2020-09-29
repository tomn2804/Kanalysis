#pragma once

#include "include/config.h"

namespace kanalysis::stats
{
	struct WeightFunction
	{
		template<typename DerivedA, typename DerivedB>
		static void sqrt_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& out);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void multiply_by_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static void multiply_by_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& in_out);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void multiply_by_sqrt_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static void multiply_by_sqrt_weights(const DenseBase<DerivedA>& sqrt_weights, DenseBase<DerivedB>& in_out);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void divide_by_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static void divide_by_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& in_out);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void divide_by_sqrt_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static void divide_by_sqrt_weights(const DenseBase<DerivedA>& sqrt_weights, DenseBase<DerivedB>& in_out);
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedA, typename DerivedB>
	void WeightFunction::sqrt_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& out)
	{
		out.derived() = weights.derived().array().sqrt();
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::multiply_by_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out)
	{
		assert(in.rows() == weights.rows());
		out.derived() = in.derived().array().colwise() * weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB>
	void WeightFunction::multiply_by_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& in_out)
	{
		assert(weights.rows() == in_out.rows());
		in_out.derived().array().colwise() *= weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::multiply_by_sqrt_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out)
	{
		assert(in.rows() == sqrt_weights.rows());
		multiply_by_weights(in, sqrt_weights, out);
	}

	template<typename DerivedA, typename DerivedB>
	void WeightFunction::multiply_by_sqrt_weights(const DenseBase<DerivedA>& sqrt_weights, DenseBase<DerivedB>& in_out)
	{
		assert(sqrt_weights.rows() == in_out.rows());
		multiply_by_weights(sqrt_weights, in_out);
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::divide_by_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out)
	{
		assert(in.rows() == weights.rows());
		out.derived() = in.derived().array().colwise() / weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB>
	void WeightFunction::divide_by_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& in_out)
	{
		assert(weights.rows() == in_out.rows());
		in_out.derived().array().colwise() /= weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::divide_by_sqrt_weights(const DenseBase<DerivedA>& in, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out)
	{
		assert(in.rows() == sqrt_weights.rows());
		divide_by_weights(in, sqrt_weights, out);
	}

	template<typename DerivedA, typename DerivedB>
	void WeightFunction::divide_by_sqrt_weights(const DenseBase<DerivedA>& sqrt_weights, DenseBase<DerivedB>& in_out)
	{
		assert(sqrt_weights.rows() == in_out.rows());
		divide_by_weights(sqrt_weights, in_out);
	}
} // namespace kanalysis::stats