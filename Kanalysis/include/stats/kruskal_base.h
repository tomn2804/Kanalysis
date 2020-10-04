#pragma once

#include "include/config.h"

#include "include/arithmetic/combination_model.h"
#include "include/arithmetic/two_hands_side_col_functor.h"
#include "include/stats/correlation.h"
#include "include/stats/correlation_weight.h"
#include "include/stats/partial_correlation.h"
#include "include/stats/partial_correlation_weight.h"
#include "include/threads/thread_pool.h"

namespace kanalysis::arithmetic
{
	namespace combination
	{
		UInt choose(int n, int r);
	} // namespace combination

	template<typename LhsMatrixType, typename RhsMatrixType>
	class TwoHandsSideColFunctor;
} // namespace kanalysis::arithmetic

namespace kanalysis::stats
{
	template<typename DerivedType>
	class KruskalBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
	public:
		template<typename Derived>
		KruskalBase(const ComputeHolderBase<Derived>& decomposition);

		template<typename Derived>
		KruskalBase(ComputeHolderBase<Derived>& decomposition);

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y, int threads) const;

		template<typename Derived>
		const Vector& solve(const VectorBase<Derived>& std_y) const;

		const Vector& partial_r_squared_sums() const;
	protected:
		KruskalBase() = default;
	private:
		template<typename Derived>
		void solve_nth_order(const VectorBase<Derived>& std_y, int threads) const;

		template<typename Derived>
		void solve_zeroth_order(const VectorBase<Derived>& std_y) const;

		int x_variables;
		int nth_order = x_variables - 1;
		arithmetic::CombinationModel m_combination_model = arithmetic::CombinationModel(x_variables, nth_order);

		mutable Vector m_partial_r_squared_sums = Vector::Constant(x_variables, 0);
		mutable Vector m_results = Vector::Constant(x_variables, 0); // Relatives

		class NthOrderFunctor
		{
		protected:
			using Iterator = discreture::Combinations<Index>::iterator;
			using ReverseIterator = discreture::Combinations<Index>::reverse_iterator;
		public:
			template<typename Derived>
			NthOrderFunctor(const KruskalBase& data, int threads, const VectorBase<Derived>& std_y, std::vector<Vector>& out);

			void operator()(Iterator lhs_first, Iterator lhs_last, ReverseIterator rhs_first, ReverseIterator rhs_last, int worker_id);
		private:
			const KruskalBase& m_data;
			using PartialCorrelationType = decltype(partial_correlation<Map<Matrix>, RegressionFunctionType>(m_data.decomposition().compute_holder(std::declval<Map<Matrix>>())));

			template<typename Derived>
			void solve_each_y(const PartialCorrelationType& pcor, const MatrixBase<Derived>& std_y, std::vector<Index>::const_iterator d_first, Vector& out);

			const Vector& m_std_y;

			std::vector<Matrix> m_lhs_model_matrices;
			std::vector<Matrix> m_rhs_model_matrices;

			std::vector<arithmetic::TwoHandsSideColFunctor<Map<Matrix>, Map<Matrix>>> m_functors;

			std::vector<PartialCorrelationType> m_lhs_pcor;
			std::vector<PartialCorrelationType> m_rhs_pcor;

			std::vector<Vector>& m_out;
		};
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::KruskalBase(const ComputeHolderBase<Derived>& decomposition)
		: Base(decomposition)
		, x_variables(Base::cols() - 1)
	{}

	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::KruskalBase(ComputeHolderBase<Derived>& decomposition)
		: Base(decomposition)
		, x_variables(Base::cols() - 1)
	{}

	template<typename DerivedType>
	template<typename Derived>
	const Vector& KruskalBase<DerivedType>::solve(const VectorBase<Derived>& std_y, int threads) const
	{
		solve_nth_order(std_y, threads);

		Array means = m_partial_r_squared_sums.array() / m_combination_model.p();
		m_results = means / means.sum();

		// If lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
		// then m_results = 0.307668, 0.19547, 0.206832, 0.0549681, 0.235061
		return m_results;
	}

	template<typename DerivedType>
	template<typename Derived>
	const Vector& KruskalBase<DerivedType>::solve(const VectorBase<Derived>& std_y) const
	{
		return solve(std_y, 1);
	}

	template<typename DerivedType>
	const Vector& KruskalBase<DerivedType>::partial_r_squared_sums() const
	{
		return m_partial_r_squared_sums;
	}

	template<typename DerivedType>
	template<typename Derived>
	void KruskalBase<DerivedType>::solve_nth_order(const VectorBase<Derived>& std_y, int threads) const
	{
		using Combination = discreture::Combinations<Index>;
		using ReverseCombination = discreture::Reversed<discreture::Combinations<Index>>;

		using Iterator = Combination::iterator;
		using ReverseIteartor = ReverseCombination::iterator;

		int packages = (m_combination_model.r() / 2) * threads;

		threads::ThreadPool pool(threads);
		pool.reserve(packages);
		pool.start();

		std::vector<std::future<void>> futures;
		futures.reserve(packages);

		std::vector<Vector> out(threads, Vector::Constant(x_variables, 0));
		NthOrderFunctor f(*this, threads, std_y, out);

		for (int nth_order = 1; nth_order <= m_combination_model.r() / 2; ++nth_order)
		{
			Combination lhs_combinations(x_variables, nth_order);
			ReverseCombination rhs_combinations(Combination(x_variables, x_variables - nth_order));

			UInt c = arithmetic::choose(x_variables, nth_order);
			int parts = (threads <= c) ? threads : c;

			std::vector<Iterator> lhs_works = discreture::divide_work_in_equal_parts(lhs_combinations.begin(), lhs_combinations.end(), parts);
			std::vector<ReverseIteartor> rhs_works = discreture::divide_work_in_equal_parts(rhs_combinations.begin(), rhs_combinations.end(), parts);

			for (int i = 0; i < parts; ++i)
			{
				futures.emplace_back(pool.submit(std::ref(f),
					lhs_works[i], lhs_works[i + 1],
					rhs_works[i], rhs_works[i + 1],
					std::placeholders::_1));
			}
		}

		// If lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
		// then solve_zeroth_order(std_y) = 9.24211, 6.99708, 7.86257, 3.1434, 7.9761
		solve_zeroth_order(std_y);

		for (auto& future : futures)
		{
			future.get();
		}

		// If m_partial_r_squared_sums.fill(0) and
		// lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
		// then m_partial_r_squared_sums = 14.9424, 8.36804, 8.39565, 1.17742, 10.5011
		for (auto& results : out)
		{
			// Reduce the results from each threads
			m_partial_r_squared_sums += results;
		}

		pool.shutdown();
	}

	template<typename DerivedType>
	template<typename Derived>
	void KruskalBase<DerivedType>::solve_zeroth_order(const VectorBase<Derived>& std_y) const
	{
		assert(m_partial_r_squared_sums.sum() == 0);

		Matrix model_matrix = Base::std_x().leftCols(2);
		Map<Matrix> std_x(model_matrix.data(), model_matrix.rows(), model_matrix.cols());

		// Fence
		auto decomposition = Base::decomposition().compute_holder(std_x);
		auto cor = correlation<Map<Matrix>, RegressionFunctionType>(decomposition);

		Scalar r = cor.solve(std_y);
		m_partial_r_squared_sums[0] += std::pow(r, 2);

		// Post
		for (Index i = 1; i < x_variables; ++i)
		{
			std_x.col(1) = Base::std_x().col(i + 1);
			cor.compute(std_x);

			r = cor.solve(std_y);
			m_partial_r_squared_sums[i] += std::pow(r, 2);
		}

		m_partial_r_squared_sums.array() *= m_combination_model.multipliers()[0];
	}

	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::NthOrderFunctor::NthOrderFunctor(const KruskalBase& data, int threads, const VectorBase<Derived>& std_y, std::vector<Vector>& out)
		: m_data(data)
		, m_std_y(std_y.derived())
		, m_lhs_model_matrices(threads, Matrix(data.std_x()))
		, m_rhs_model_matrices(threads, Matrix(data.std_x()))
		, m_out(out)
	{
		m_functors.reserve(threads);
		m_lhs_pcor.reserve(threads);
		m_rhs_pcor.reserve(threads);

		for (int i = 0; i < threads; ++i)
		{
			Map<Matrix> lhs_std_x(m_lhs_model_matrices[i].data(), m_lhs_model_matrices[i].rows(), m_lhs_model_matrices[i].cols());
			Map<Matrix> rhs_std_x(m_rhs_model_matrices[i].data(), m_rhs_model_matrices[i].rows(), m_rhs_model_matrices[i].cols());

			auto lhs_decomposition = m_data.decomposition().compute_holder(lhs_std_x);
			m_lhs_pcor.emplace_back(lhs_decomposition);

			auto rhs_decomposition = m_data.decomposition().compute_holder(rhs_std_x);
			m_rhs_pcor.emplace_back(rhs_decomposition);

			Map<Matrix> lhs_iteration(lhs_std_x.col(1).data(), lhs_std_x.rows(), lhs_std_x.cols() - 1);
			Map<Matrix> rhs_iteration(rhs_std_x.col(1).data(), rhs_std_x.rows(), rhs_std_x.cols() - 1);
			m_functors.emplace_back(arithmetic::TwoHandsSideColFunctor<Map<Matrix>, Map<Matrix>>(lhs_iteration, rhs_iteration));
		}
	}

	template<typename DerivedType>
	void KruskalBase<DerivedType>::NthOrderFunctor::operator()(Iterator lhs_first, Iterator lhs_last, ReverseIterator rhs_first, ReverseIterator rhs_last, int worker_id)
	{
		Index l = discreture::Combinations<Index>::get_index(*lhs_first);
		Index c = discreture::Combinations<Index>::get_index(*lhs_last);

		PartialCorrelationType& local_lhs_pcor = m_lhs_pcor[worker_id];
		PartialCorrelationType& local_rhs_pcor = m_rhs_pcor[worker_id];

		Map<Matrix>& lhs_std_x = local_lhs_pcor.const_cast_std_x();
		new (&lhs_std_x) Map<Matrix>(lhs_std_x.data(), lhs_std_x.rows(), lhs_first->size() + 1);

		Map<Matrix>& rhs_std_x = local_rhs_pcor.const_cast_std_x();
		new (&rhs_std_x) Map<Matrix>(rhs_std_x.data(), rhs_std_x.rows(), rhs_first->size() + 1);

		arithmetic::TwoHandsSideColFunctor<Map<Matrix>, Map<Matrix>>& local_functor = m_functors[worker_id];
		local_functor.reset(*lhs_first, *rhs_first);

		Vector& local_out = m_out[worker_id];

		for (; lhs_first != lhs_last; ++lhs_first, ++rhs_first)
		{
			local_functor(*lhs_first, *rhs_first);

			// lhs as model matrix
			local_lhs_pcor.compute(lhs_std_x);
			solve_each_y(local_lhs_pcor, local_functor.rhs().matrix(), rhs_first->cbegin(), local_out);

			// rhs as model matrix
			local_rhs_pcor.compute(rhs_std_x);
			solve_each_y(local_rhs_pcor, local_functor.lhs().matrix(), lhs_first->cbegin(), local_out);
		}
		assert(rhs_first == rhs_last);
	}

	template<typename DerivedType>
	template<typename Derived>
	void KruskalBase<DerivedType>::NthOrderFunctor::solve_each_y(const PartialCorrelationType& pcor, const MatrixBase<Derived>& std_y, std::vector<Index>::const_iterator d_first, Vector& out)
	{
		// Fence
		Scalar r = pcor.solve(std_y.derived().col(0), m_std_y);
		out[*d_first] += std::pow(r, 2) * m_data.m_combination_model.multipliers()[pcor.cols() - 1];
		++d_first;

		// Post
		for (Index i = 1; i < std_y.cols(); ++i, ++d_first)
		{
			r = pcor.solve(std_y.derived().col(i));
			out[*d_first] += std::pow(r, 2) * m_data.m_combination_model.multipliers()[pcor.cols() - 1];
		}
	}
} // namespace kanalysis::stats
