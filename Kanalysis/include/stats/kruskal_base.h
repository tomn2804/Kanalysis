#pragma once

#include "include/config.h"

#include "include/stats/solve_holder_base.h"

namespace kanalysis::arithmetic
{
	namespace combination
	{
		UInt choose(int n, int r);
	} // namespace combination

	template<typename MapType>
	class ColFunctor;

	template<typename FunctorType>
	class CombinationFunctorIterator;

	class CombinationModel;

	template<typename LhsMatrixType, typename RhsMatrixType>
	class TwoHandsSideColFunctor;
} // namespace kanalysis::arithmetic

namespace kanalysis::threads
{
	class ThreadPool;
} // namespace kanalysis::threads

namespace kanalysis::utils::console
{
	class Progress;
} // namespace kanalysis::utils::console

namespace kanalysis::stats
{
	template<typename DerivedType>
	class KruskalBase : public SolveHolderBase<DerivedType>
	{
	protected:
		using Base = SolveHolderBase<DerivedType>;
		using typename Base::ComputeHolderDecayType;
		using typename Base::RegressionFunctionType;
		using typename Base::MatrixType;

		template<typename MatrixType_, typename ArrayType_>
		using ComputeHolderTemplateType = typename ComputeHolderTraits<ComputeHolderDecayType>::template ComputeHolderTemplateType<MatrixType_, ArrayType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using CorrelationTemplateType = typename ComputeHolderTraits<ComputeHolderDecayType>::template CorrelationTemplateType<ComputeHolderType_, RegressionFunctionType_>;

		template<typename ComputeHolderType_, typename RegressionFunctionType_>
		using PartialCorrelationTemplateType = typename ComputeHolderTraits<ComputeHolderDecayType>::template PartialCorrelationTemplateType<ComputeHolderType_, RegressionFunctionType_>;
	public:
		template<typename Derived>
		KruskalBase(const ComputeHolderBase<Derived>& compute_holder);

		template<typename Derived>
		KruskalBase(ComputeHolderBase<Derived>& compute_holder);

		template<typename Derived>
		const Vector& standardized_solve(const VectorBase<Derived>& standardized_y, int threads);

		template<typename Derived>
		const Vector& standardized_solve(const VectorBase<Derived>& standardized_y);

		const Vector& partial_r_squared_sums() const;
	protected:
		KruskalBase() = default;
	private:
		class Formula
		{
		protected:
			using ComputeHolderType = ComputeHolderTemplateType<Map<Matrix>, Array>;
			using PartialCorrelationType = PartialCorrelationTemplateType<ComputeHolderType, RegressionFunctionType>;
			using ColFunctorType = arithmetic::ColFunctor<Map<Matrix>>;
			using ColIteratorType = arithmetic::CombinationFunctorIterator<ColFunctorType>;

			using Iterator = discreture::Combinations<Index>::iterator;
			using ReverseIterator = discreture::Combinations<Index>::reverse_iterator;
		public:
			template<typename Derived>
			Formula(KruskalBase& kruskal, int threads, const VectorBase<Derived>& standardized_y, std::vector<Vector>& out);

			void operator()(Iterator lhs_first, Iterator lhs_last, ReverseIterator rhs_first, ReverseIterator rhs_last, int worker_id);
		private:
			KruskalBase& m_kruskal;
			const Vector& m_standardized_y;

			std::vector<Matrix> m_standardized_model_matrices;
			std::vector<PartialCorrelationType> m_partial_correlations;
			std::vector<ColIteratorType> m_iterators;

			std::vector<Vector>& m_out;
		};

		template<typename Derived>
		void solve_nth_order(const VectorBase<Derived>& standardized_y, int threads);

		template<typename Derived>
		void solve_zeroth_order(const VectorBase<Derived>& standardized_y);

		int x_variables;
		int nth_order = x_variables - 1;
		arithmetic::CombinationModel m_combination_model = arithmetic::CombinationModel(x_variables, nth_order);

		Vector m_partial_r_squared_sums = Vector::Constant(x_variables, 0);
		Vector m_results = Vector::Constant(x_variables, 0); // Relatives

		utils::console::Progress m_progress_status = utils::console::Progress(m_combination_model.cumulative_c());
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::KruskalBase(const ComputeHolderBase<Derived>& compute_holder)
		: Base(compute_holder)
		, x_variables(Base::cols() - 1)
	{}

	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::KruskalBase(ComputeHolderBase<Derived>& compute_holder)
		: Base(compute_holder)
		, x_variables(Base::cols() - 1)
	{}

	template<typename DerivedType>
	template<typename Derived>
	const Vector& KruskalBase<DerivedType>::standardized_solve(const VectorBase<Derived>& standardized_y, int threads)
	{
		m_progress_status.start();

		solve_nth_order(standardized_y, threads);

		Array means = m_partial_r_squared_sums.array() / m_combination_model.p();
		m_results = means / means.sum();

		m_progress_status.stop();

		// If lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
		// then m_results = 0.307668, 0.19547, 0.206832, 0.0549681, 0.235061
		return m_results;
	}

	template<typename DerivedType>
	template<typename Derived>
	const Vector& KruskalBase<DerivedType>::standardized_solve(const VectorBase<Derived>& standardized_y)
	{
		return standardized_solve(standardized_y, 1);
	}

	template<typename DerivedType>
	const Vector& KruskalBase<DerivedType>::partial_r_squared_sums() const
	{
		return m_partial_r_squared_sums;
	}

	template<typename DerivedType>
	template<typename Derived>
	void KruskalBase<DerivedType>::solve_nth_order(const VectorBase<Derived>& standardized_y, int threads)
	{
		using Iterator = discreture::Combinations<Index>::iterator;
		using ReverseIterator = discreture::Combinations<Index>::reverse_iterator;

		int packages = m_combination_model.r() * threads;

		threads::ThreadPool pool(threads);
		pool.reserve(packages);
		pool.start();

		std::vector<std::future<void>> futures;
		futures.reserve(packages);

		std::vector<Vector> out(threads, Vector::Constant(x_variables, 0));
		Formula functor(*this, threads, standardized_y, out);

		auto f = [&](Iterator lhs_first, Iterator lhs_last, ReverseIterator rhs_first, ReverseIterator rhs_last)
		{
			Matrix lhs_matrix(Base::std_matrix());
			Matrix rhs_matrix(Base::std_matrix());

			Map<Matrix> lhs_map(lhs_matrix.data(), lhs_matrix.rows(), lhs_matrix.cols());
			Map<Matrix> rhs_map(rhs_matrix.data(), rhs_matrix.rows(), rhs_matrix.cols());

			PartialCorrelationTemplateType<ComputeHolderTemplateType<Map<Matrix>, Array>, RegressionFunctionType> lhs_pcor(ComputeHolderTemplateType<Map<Matrix>, Array>(lhs_map));

			arithmetic::TwoHandsSideColFunctor<Map<Matrix>, Map<Matrix>> matrices(lhs_map, rhs_map);
			matrices.reset(*lhs_first, *rhs_first);

			for (; lhs_first != lhs_last; ++lhs_first, ++rhs_first)
			{
				matrices(*lhs_first, *rhs_first);
			}
		}

		for (int nth_order = m_combination_model.r(); nth_order > 0; --nth_order)
		{
			auto lhs_combinations = discreture::combinations(x_variables, nth_order);
			auto rhs_combinations = discreture::reversed(discreture::combinations(x_variables, x_variables - nth_order));

			auto lhs_works = discreture::divide_work_in_equal_parts(lhs_combinations, threads);
			auto rhs_works = discreture::divide_work_in_equal_parts(rhs_combinations, threads);

			for (int i = 0; i < threads; ++i)
			{
				pool.submit(functor,
							lhs_works[i], lhs_works[i + 1],
							rhs_works[i], rhs_works[i + 1],
							std::placeholders::_1);
			}
		}

		// If lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
		// then solve_zeroth_order(standardized_y) = 9.24211, 6.99708, 7.86257, 3.1434, 7.9761
		solve_zeroth_order(standardized_y);

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
	void KruskalBase<DerivedType>::solve_zeroth_order(const VectorBase<Derived>& standardized_y)
	{
		assert(m_partial_r_squared_sums.sum() == 0);

		Matrix standardized_model_matrix = Base::std_matrix().leftCols(2);

		// Fence
		ComputeHolderTemplateType<Matrix, Array> compute_holder(standardized_model_matrix, Base::compute_holder().weights(), true);
		CorrelationTemplateType<ComputeHolderTemplateType<Matrix, Array>, RegressionFunctionType> correlation(compute_holder);
		Scalar r = correlation.standardized_solve(standardized_y);
		m_partial_r_squared_sums[0] += std::pow(r, 2);

		// Post
		for (Index i = 1; i < x_variables; ++i)
		{
			standardized_model_matrix.col(1) = Base::std_matrix().col(i + 1);
			correlation.std_compute(standardized_model_matrix);
			r = correlation.standardized_solve(standardized_y);
			m_partial_r_squared_sums[i] += std::pow(r, 2);
		}

		m_partial_r_squared_sums.array() *= m_combination_model.multipliers()[0];
	}

	template<typename DerivedType>
	template<typename Derived>
	KruskalBase<DerivedType>::Formula::Formula(KruskalBase& kruskal, int threads, const VectorBase<Derived>& standardized_y, std::vector<Vector>& out)
		: m_kruskal(kruskal)
		, m_standardized_y(standardized_y.derived())
		, m_standardized_model_matrices(threads, Matrix(kruskal.std_matrix()))
		, m_out(out)
	{
		m_partial_correlations.reserve(threads);
		m_iterators.reserve(threads);

		for (auto& mx : m_standardized_model_matrices)
		{
			Map<Matrix> standardized_model_matrix(mx.data(), mx.rows(), mx.cols());
			m_partial_correlations.emplace_back(ComputeHolderType(standardized_model_matrix, m_kruskal.weights(), true));

			Map<Matrix> iterator_matrix(standardized_model_matrix.col(1).data(), standardized_model_matrix.rows(), standardized_model_matrix.cols() - 1);
			m_iterators.emplace_back(ColIteratorType(ColFunctorType(iterator_matrix), iterator_matrix.cols()));
		}
	}

	template<typename DerivedType>
	void KruskalBase<DerivedType>::Formula::operator()(int worker_id, int nth_order, UInt k, UInt l)
	{
		PartialCorrelationType& pcor = m_partial_correlations[worker_id];

		Map<Matrix>& standardized_model_matrix = pcor.const_cast_std_matrix();
		new (&standardized_model_matrix) Map<Matrix>(standardized_model_matrix.data(), standardized_model_matrix.rows(), nth_order + 1);

		ColIteratorType& iterator = m_iterators[worker_id];
		iterator.reset(nth_order, k, l);

		Vector& out = m_out[worker_id];

		const MatrixType& read_only_model_mx = m_kruskal.std_matrix();
		while (iterator.has_next())
		{
			pcor.std_compute(standardized_model_matrix);

			// Fence
			auto i = iterator.indicies().middle();
			Scalar r = pcor.standardized_solve(read_only_model_mx.col(*i + 1), m_standardized_y);
			out[*i] += std::pow(r, 2) * m_kruskal.m_combination_model.multipliers()[nth_order];
			++i;

			// Post
			for (; i != iterator.indicies().end(); ++i)
			{
				r = pcor.standardized_solve(read_only_model_mx.col(*i + 1));
				out[*i] += std::pow(r, 2) * m_kruskal.m_combination_model.multipliers()[nth_order];
			}

			iterator.next();
		}

		m_kruskal.m_progress_status.update(l - k);
	}
} // namespace kanalysis::stats

	//template<typename DerivedType>
	//template<typename Derived>
	//void KruskalBase<DerivedType>::solve_nth_order(const VectorBase<Derived>& standardized_y, int threads)
	//{
	//	int packages = m_combination_model.r() * threads;

	//	threads::ThreadPool pool(threads);
	//	pool.reserve(packages);
	//	pool.start();

	//	std::vector<std::future<void>> futures;
	//	futures.reserve(packages);

	//	std::vector<Vector> out(threads, Vector::Constant(x_variables, 0));
	//	Formula functor(*this, threads, standardized_y, out);

	//	UInt tasks;
	//	int remain_tasks;
	//	UInt tasks_per_thread;

	//	// If x_variables = 5, and nth_order = 4.
	//	// Then at start loop
	//	// nth_order = 4; [first, middle) -> 0 1 2 3
	//	// nth_order = 3; [first, middle) -> 0 1 2
	//	// nth_order = 2; [first, middle) -> 0 1
	//	// nth_order = 1; [first, middle) -> 0
	//	// End of loop
	//	for (int nth_order = m_combination_model.r(); nth_order > 0; --nth_order)
	//	{
	//		assert(m_combination_model.n() == x_variables);

	//		// Tasks distribution calculation
	//		tasks = arithmetic::combination::choose(m_combination_model.n(), nth_order);
	//		if (threads > tasks)
	//		{
	//			remain_tasks = 0;
	//			tasks_per_thread = 1;
	//		}
	//		else
	//		{
	//			remain_tasks = tasks % threads;
	//			tasks_per_thread = (tasks - remain_tasks) / threads;
	//		}

	//		// Tasks splitting
	//		UInt k = 0;
	//		UInt l = tasks_per_thread;
	//		for (; k < tasks; k = l, l += tasks_per_thread)
	//		{
	//			if (remain_tasks > 0)
	//			{
	//				++l;
	//				--remain_tasks;
	//			}
	//			futures.emplace_back(pool.submit(std::ref(functor), std::placeholders::_1, nth_order, k, l));
	//		}

	//		assert(remain_tasks == 0);
	//		assert(l - tasks_per_thread == tasks);
	//	}

	//	// If lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
	//	// then solve_zeroth_order(standardized_y) = 9.24211, 6.99708, 7.86257, 3.1434, 7.9761
	//	solve_zeroth_order(standardized_y);

	//	for (auto& future : futures)
	//	{
	//		future.get();
	//	}

	//	// If m_partial_r_squared_sums.fill(0) and
	//	// lm(q101 ~ q104 + q105 + q106 + q107, data = data, weights = w),
	//	// then m_partial_r_squared_sums = 14.9424, 8.36804, 8.39565, 1.17742, 10.5011
	//	for (auto& results : out)
	//	{
	//		// Reduce the results from each threads
	//		m_partial_r_squared_sums += results;
	//	}

	//	pool.shutdown();
	//}