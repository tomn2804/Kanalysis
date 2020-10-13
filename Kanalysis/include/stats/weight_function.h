#pragma once

#include "config.h"

namespace kanalysis::stats
{
	struct WeightFunction
	{
		template<typename DerivedA, typename DerivedB>
		static void sqrt_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Array sqrt_weights(const DenseBase<Derived>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void multiply_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static Matrix multiply_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void multiply_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static Matrix multiply_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void divide_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static Matrix divide_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void divide_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static Matrix divide_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		static void standardize(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out);

		template<typename DerivedA, typename DerivedB>
		static Matrix standardize(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights);
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedA, typename DerivedB>
	void WeightFunction::sqrt_weights(const DenseBase<DerivedA>& weights, DenseBase<DerivedB>& out)
	{
		out.derived() = weights.derived().array().sqrt();
	}

	template<typename Derived>
	Array WeightFunction::sqrt_weights(const DenseBase<Derived>& weights)
	{
		Array out;
		sqrt_weights(weights, out);
		return out;
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::multiply_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out)
	{
		assert(x.rows() == weights.rows());
		out.derived() = x.derived().array().colwise() * weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB>
	Matrix WeightFunction::multiply_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights)
	{
		Matrix out;
		multiply_by_weights(x, weights, out);
		return out;
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::multiply_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out)
	{
		assert(x.rows() == sqrt_weights.rows());
		multiply_by_weights(x, sqrt_weights, out);
	}

	template<typename DerivedA, typename DerivedB>
	Matrix WeightFunction::multiply_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights)
	{
		Matrix out;
		multiply_by_sqrt_weights(x, sqrt_weights, out);
		return out;
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::divide_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out)
	{
		assert(x.rows() == weights.rows());
		out.derived() = x.derived().array().colwise() / weights.derived().array();
	}

	template<typename DerivedA, typename DerivedB>
	Matrix WeightFunction::divide_by_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights)
	{
		Matrix out;
		divide_by_weights(x, weights, out);
		return out;
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::divide_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights, DenseBase<DerivedC>& out)
	{
		assert(x.rows() == sqrt_weights.rows());
		divide_by_weights(x, sqrt_weights, out);
	}

	template<typename DerivedA, typename DerivedB>
	Matrix WeightFunction::divide_by_sqrt_weights(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& sqrt_weights)
	{
		Matrix out;
		divide_by_sqrt_weights(x, sqrt_weights, out);
		return out;
	}

	template<typename DerivedA, typename DerivedB, typename DerivedC>
	void WeightFunction::standardize(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights, DenseBase<DerivedC>& out)
	{
		assert(x.rows() == weights.rows());
		multiply_by_sqrt_weights(x, sqrt_weights(weights), out);
	}

	template<typename DerivedA, typename DerivedB>
	Matrix WeightFunction::standardize(const DenseBase<DerivedA>& x, const DenseBase<DerivedB>& weights)
	{
		Matrix out;
		standardize(x, weights, out);
		return out;
	}
} // namespace kanalysis::stats
