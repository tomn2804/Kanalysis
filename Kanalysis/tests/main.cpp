#include "include/config.h"

#include <discreture.hpp>
//#include "include/kanalysis.h"
#include "tests/tests.h"
//#include "include/arithmetic/col_functor.h"
//
//#include "include/threads/thread_pool.h"
//
//#include "include/arithmetic/two_hands_side_col_functor.h"

//using discreture::operator<<;

//class Functor
//{
//public:
//	int operator()(discreture::Combinations<int>::iterator first1, discreture::Combinations<int>::iterator last1,
//				   discreture::Combinations<int>::reverse_iterator first2, discreture::Combinations<int>::reverse_iterator last2,
//				   int worker_id)
//	{
//		assert(std::distance(first1, last1) == std::distance(first2, last2));
//
//		using discreture::combinations;
//
//		std::unique_lock<std::mutex> lock(m_mutex);
//
//		for (; first1 != last1; ++first1, ++first2)
//		{
//			std::cout << *first1 << ' ';
//			std::cout << *first2 << '\n';
//		}
//
//		return ++q;
//	}
//	int q = 0;
//	std::mutex m_mutex;
//};
//
//class FormulaFunctor
//{
//public:
//	void operator()(int i)
//	{
//		std::unique_lock<std::mutex> lock(m_mutex);
//
//		//std::cout << i << '\n';
//	}
//	std::mutex m_mutex;
//};
//
#include <discreture.hpp>
#include "include/stats/compute_holder.h"
#include "include/stats/compute_holder_weight.h"
#include "include/stats/weight_function.h"
#include "include/stats/coefficient.h"
#include "include/stats/linear_regression_function.h"
#include "include/stats/coefficient_weight.h"
#include "include/stats/fitted_value.h"
#include "include/stats/fitted_value_weight.h"
#include "include/stats/residual.h"
#include "include/stats/residual_weight.h"
#include "include/stats/correlation.h"
#include "include/stats/correlation_weight.h"
#include "include/stats/partial_correlation.h"
#include "include/stats/partial_correlation_weight.h"

void test()
{
	using namespace kanalysis;
	using namespace kanalysis::stats;
	using namespace kanalysis::tests;

	int threads = 8;

	{
		int variables = 5;

		std::string filename = DATA_DIRECTORY;
		filename += "test_data_1.csv";
		Data data(filename, variables);

		Matrix temp = data.model_matrix();
		Matrix model_matrix = temp.leftCols(temp.cols() - 1);
		Vector y = data.y();

		std::cout << "Number of Variables: " << variables << '\n';
		std::cout << "Number of Threads: " << threads << '\n';
		std::cout << "Unweighted:" << '\n';
		{
			DEBUG_TIME();

			ComputeHolder<Matrix> c(model_matrix);
			PartialCorrelation<ComputeHolder<Matrix>, LinearRegressionFunction> k(c);
			std::cout << k.solve(y, temp.col(temp.cols() - 1)) << '\n';
		}
		std::system("pause");

		std::cout << "Weighted:" << '\n';
		{
			DEBUG_TIME();
			Matrix std_matrix = WeightFunction::standardize(model_matrix, data.weights());
			ComputeHolderWeight<Matrix, Array> c(std_matrix, data.weights());
			PartialCorrelationWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
			std::cout << k.solve(c.standardize(y), c.standardize(temp.col(temp.cols() - 1))) << '\n';
		}
		std::system("pause");
	}
}

int main()
{
	using namespace kanalysis;
	using namespace kanalysis::stats;
	//using namespace kanalysis::utils;
	//using namespace kanalysis::arithmetic;

	Matrix mx = Matrix::Constant(5, 5, 3);
	Vector w = Vector::Constant(5, 2);
	Vector out = Vector::Constant(5, 2);

	test();

	//ComputeHolder<Matrix> ye(mx);
	////Ref<Matrix> temp = mx;
	////auto huh = coefficient<Ref<Matrix>, LinearRegressionFunction>(ye.compute_holder(temp));

	//Map<Matrix> ye2(mx.data(), 5, 5);
	//auto z = coefficient<Map<Matrix>, LinearRegressionFunction>(ye.compute_holder(ye2));
	//auto huh = discreture::combinations(z);

	//{
	//	Matrix mx = Matrix::Constant(5, 5, 3);
	//	Vector w = Vector::Constant(5, 2);
	//	Vector out = Vector::Constant(5, 2);

	//	ComputeHolder<Ref<Matrix>> cc(mx);

	//	Ref<Matrix> mxx = mx;
	//	Vector zz = cc.standardize(w);

	//	mxx.fill(99);
	//	std::cout << zz << '\n';
	//	auto ye = cc.compute_holder(cc.standardize(mx));
	//}

	//{
	//	Matrix mx = Matrix::Constant(5, 5, 3);
	//	Vector w = Vector::Constant(5, 2);

	//	Matrix ye = WeightFunction::divide_by_sqrt_weights(mx, w);

	//	ComputeHolderWeight<Ref<Matrix>, Array> cc(mx, w);

	//	Ref<Matrix> mxx = mx;
	//	cc.standardize(mx, mxx);
	//	auto ye = cc.compute_holder(mxx);
	//	mx.fill(123);
	//	std::cout << ye.std_matrix() << '\n';
	//}
	//Matrix mx(1, 5);
	//std::iota(mx.data(), mx.data() + mx.size(), 0);

	//kanalysis::arithmetic::TwoHandsSideColFunctor<Matrix, Matrix> col(mx, mx);

	//auto lhs = discreture::combinations(mx.cols(), mx.cols() / 2);
	//auto rhs = discreture::reversed(discreture::combinations(mx.cols(), mx.cols() - mx.cols() / 2));

	//auto test = discreture::divide_work_in_equal_parts(lhs, 4);

	//{
	//	DEBUG_TIME();
	//	auto lhs_first = lhs.begin();
	//	auto lhs_last = lhs.end();
	//	auto rhs_first = rhs.begin();

	//	col.reset(*lhs_first, *rhs_first);

	//	for (; lhs_first != lhs_last; ++lhs_first, ++rhs_first)
	//	{
	//		col(*lhs_first, *rhs_first);
	//		std::cout << col.lhs().matrix() << '\n';
	//		std::cout << col.rhs().matrix() << '\n';
	//		std::cout << '\n';
	//	}
	//}

	//kanalysis::arithmetic::TwoHandsSideColFunctor<Matrix, discreture::Combinations<Index>> col(mx);
	//discreture::Combinations temp(mx.cols(), mx.cols() / 2);
	//col.reset(*temp.begin(), temp.get_n(), temp.get_k());

	//{
	//	DEBUG_TIME();
	//	auto first = temp.begin();
	//	auto last = temp.end();
	//	for (; first != last; ++first)
	//	{
	//		col(*first);
	//		std::cout << col.lhs().matrix() << '\n';
	//		std::cout << col.rhs().matrix() << '\n';
	//		std::cout << '\n';
	//	}
	//}

	//auto a = []() { return Vector(5); };

	//int variables = 5;
	//int nth_order = 4;
	//int processors = 2;

	//int packages = nth_order * processors;
	//std::vector<std::future<void>> futures;
	//futures.reserve(packages);

	//Functor f;
	//FormulaFunctor ff;

	//auto custom2 = [&](discreture::Combinations<int>::iterator first1, discreture::Combinations<int>::iterator last1,
	//				   discreture::Combinations<int>::reverse_iterator first2, discreture::Combinations<int>::reverse_iterator last2,
	//				   int worker_id)
	//{
	//	ff(f(first1, last1, first2, last2, worker_id));

	//	auto rfirst = std::make_reverse_iterator(first1);
	//};

	//threads::ThreadPool pool(processors);
	//pool.start();

	//for (int r = 4; r > 0; --r)
	//{
	//	auto lhs_combinatons = discreture::combinations(variables, r);
	//	auto rhs_combination = discreture::reversed(discreture::combinations(variables, variables - r));

	//	auto lhs_works = discreture::divide_work_in_equal_parts(lhs_combinatons, processors);
	//	auto rhs_works = discreture::divide_work_in_equal_parts(rhs_combination, processors);

	//	for (int i = 0; i < processors; ++i)
	//	{
	//		futures.emplace_back(pool.submit(custom2,
	//										 lhs_works[i], lhs_works[i + 1],
	//										 rhs_works[i], rhs_works[i + 1],
	//										 std::placeholders::_1));
	//	}
	//	std::system("pause");
	//}

	//for (auto& f : futures)
	//{
	//	f.get();
	//}

	//pool.shutdown();

	std::system("pause");
}

//void test()
//{
//	//using namespace kanalysis::arithmetic;
//	using namespace kanalysis::stats;
//	using namespace kanalysis::tests;
//
//	int threads = 8;
//
//	{
//		int variables = 5;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_1.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			// Correct results: 0.320966, 0.190873, 0.209141, 0.037046, 0.241973
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			// Correct results: 0.30766828 0.19547034 0.20683219 0.05496811 0.23506109
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 10;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_2.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 15;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_2.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 18;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_2.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 20;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_3.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 25;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_3.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//
//	{
//		int variables = 30;
//		int threads = 4;
//
//		std::string filename = DATA_DIRECTORY;
//		filename += "test_data_3.csv";
//		Data data(filename, variables);
//
//		Matrix model_matrix = data.model_matrix();
//		Vector y = data.y();
//
//		std::cout << "Number of Variables: " << variables << '\n';
//		std::cout << "Number of Threads: " << threads << '\n';
//		std::cout << "Unweighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolder<Matrix, Array> c(model_matrix);
//			Kruskal<ComputeHolder<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//
//		std::cout << "Weighted:" << '\n';
//		{
//			DEBUG_TIME();
//
//			ComputeHolderWeight<Matrix, Array> c(model_matrix, data.weights());
//			KruskalWeight<ComputeHolderWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//			std::cout << k.solve(y, 4) << '\n';
//		}
//	}
//	std::cout << "\n\n";
//	std::system("pause");
//}

//template<typename DerivedA, typename DerivedB>
//Scalar test(const MatrixBase<DerivedA>& mx, const VectorBase<DerivedB>& y)
//{
//	Decomposition<Matrix> qr(mx.derived());
//	Vector x_hat = qr.solve(y.derived());
//	Matrix ye = qr.matrixQR().triangularView<Eigen::Upper>();
//	Vector res = (qr.householderQ() * ye * x_hat);
//	return res.sum();
//}
//
//template<typename DerivedA, typename DerivedB>
//Scalar test2(const MatrixBase<DerivedA>& mxx, const VectorBase<DerivedB>& y)
//{
//	Matrix mx = mxx.derived();
//	Decomposition<Matrix> qr(mx);
//	Vector x_hat = qr.solve(y.derived());
//	Vector res = (mx * x_hat);
//	return res.sum();
//}

//template<typename DerivedA, typename DerivedB, typename DerivedC>
//Scalar CorrelationWeight::cor(const VectorBase<DerivedA>& fitted_values, const VectorBase<DerivedB>& residuals, const VectorBase<DerivedC>& weights) const
//{
//	Scalar sum_weights = weights.sum();
//
//	Vector weighted_fitted_values = weights.derived().array() * fitted_values.derived().array() / sum_weights;
//	Scalar mean = weighted_fitted_values.sum();
//
//	Scalar mss = (weights.derived().array() - fitted_values.derived().array() - mean).array().square().sum();
//	Scalar rss = (weights.derived().array() * residuals.array().square()).sum();
//
//	return mss / (mss + rss);
//}