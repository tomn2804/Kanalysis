#include "kanalysis.h"

using namespace kanalysis;
using namespace kanalysis::stats;
using namespace kanalysis::utils;

namespace kanalysis
{
	template<typename MatrixType>
	class Weight
	{
	public:
		template<typename Derived>
		Weight(const DenseBase<Derived>& x)
			: data2(x.derived())
		{}

		template<typename Derived>
		Weight(DenseBase<Derived>& x)
			: data2(x.derived())
		{}

		MatrixType& data()
		{
			return data2;
		}

		const MatrixType& operator=(const Weight& other) const
		{
			return data2;
		}

		MatrixType& operator=(Weight& other)
		{
			return data2;
		}

		MatrixType data2;
	};

	template<typename Derived>
	void test(const Matrix& x)
	{
		return x;
	}
}

int main()
{
	Matrix ye;

	Weight<Matrix> huh(ye);

	Matrix huh2 = huh.data();
	test(huh);

	//Matrix test = Matrix::Random(5, 5);
	//test = test.array().abs2().round();
	//std::cout << test << '\n';

	//Matrix data = read_csv(TITANIC_FILE_PATH);

	//Vector y = data.col(0);
	//Matrix x = as_model_matrix(data.rightCols(data.cols() - 1));

	//auto qr = decomposition(x);
	//std::cout << kruskal(qr).solve(y) << '\n';

	//Matrix x(100, 5); // Data with 5 independent variables, and 7 observations
	//for (Index i = 0; i < x.cols(); ++i)
	//{
	//	x.col(i).fill(i + 40);
	//}

	//Vector y = Vector::Constant(100, 20); // Dependent variable
	//Array w = Array::Constant(100, 1); // Weights

	//Matrix std_x = WeightFunction::standardize(x, w);
	//Matrix std_y = WeightFunction::standardize(y, w);

	//Vector x1 = x.col(0); // Takes the first independent variable x1 out, to be correlate with y.
	//Matrix x_to_ctrl = x.rightCols(x.cols() - 1);

	//std::cout << "x1" << std::endl;
	//std::cout << "--" << std::endl;
	//std::cout << x1 << std::endl << std::endl;

	//std::cout << "x2 x3 x4 x5" << std::endl;
	//std::cout << "-----------" << std::endl;
	//std::cout << x_to_ctrl << std::endl;

	//int threads = 8; // Number of threads to be run in parallel

	//auto solver = kruskal(decomposition(x)); // Returns a Kruskal object
	//Vector relative_importances = solver.solve(y, threads); // Finds the relative importances each x variables toward y

	//auto weighted_solver = kruskal(decomposition(std_x, w)); // Returns a KruskalWeight object
	//Vector weighted_relative_importances = weighted_solver.solve(y, threads); // Finds the relative importances each x variables toward y

	//std::cout << relative_importances << std::endl << std::endl;
	//std::cout << weighted_relative_importances << std::endl;

	std::system("pause");
}

//#include "include/config.h"
//
//#include <discreture.hpp>
//#include "include/kanalysis.h"
//#include "tests/tests.h"
//
//using discreture::operator<<;
//
//int main()
//{
//	using namespace kanalysis;
//	using namespace kanalysis::stats;
//	using namespace kanalysis::tests;
//
//	//A q = test(5);
//	//std::cout << q.i << '\n';
//	//std::system("pause");
//
//	//using namespace kanalysis::utils;
//	//using namespace kanalysis::arithmetic;
//
//	//using namespace discreture;
//	////Matrix mx = Matrix::Constant(5, 5, 3);
//	////Vector w = Vector::Constant(5, 2);
//	////Vector out = Vector::Constant(5, 2);
//	//int n = 5;
//	//int k = 4;
//	//int threads = 5;
//	//Combinations<Index> c(n, k);
//	//auto works = discreture::divide_work_in_equal_parts(c.begin(), c.end(), threads);
//	//std::cout << works.size() << '\n';
//
//	//for (size_t i = 0; i < threads; ++i)
//	//{
//	//	auto local_first = works[i];
//	//	auto local_last = works[i + 1];
//
//	//	//std::cout << *local_first << '\n';
//	//	for (; local_first != local_last; ++local_first)
//	//	{
//	//		std::cout << *local_first << '\n';
//	//	}
//	//	std::cout << "i\n";
//	//}
//
//	//int threads = 8;
//
//	Matrix data = Matrix::Random(100, 5);
//	Vector y = Vector::Random(100);
//
//	Matrix x = utils::as_model_matrix(data);
//	std::cout << x << '\n';
//	Decomposition<Matrix> qr(x);
//	auto k = kruskal(qr);
//
//	int threads = 8;
//	Vector results = k.solve(y, threads);
//
//	auto hih = coefficient(x, y);
//
//	std::cout << results << std::endl;
//
//	//{
//	//	int variables = 5;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_1.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		// Correct results: 0.30766828 0.19547034 0.20683219 0.05496811 0.23506109
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//{
//	//	int variables = 10;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_2.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//{
//	//	int variables = 18;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_2.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//{
//	//	int variables = 20;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_3.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//{
//	//	int variables = 25;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_3.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//{
//	//	int variables = 30;
//
//	//	std::string filename = DATA_DIRECTORY;
//	//	filename += "test_data_3.csv";
//	//	Data data(filename, variables);
//
//	//	Matrix model_matrix = data.model_matrix();
//	//	Vector y = data.y();
//
//	//	std::cout << "Number of Variables: " << variables << '\n';
//	//	std::cout << "Number of Threads: " << threads << '\n';
//
//	//	{
//	//		DEBUG_TIME();
//
//	//		Matrix mx = WeightFunction::standardize(model_matrix, data.weights());
//	//		DecompositionWeight<Matrix, Array> c(mx, data.weights());
//	//		KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
//	//		Vector yy = c.standardize(y);
//	//		std::cout << k.solve(yy, threads) << '\n';
//	//	}
//	//}
//
//	//auto rhs = discreture::reversed(discreture::combinations(mx.cols(), mx.cols() - mx.cols() / 2));
//
//	//{
//	//	auto lhs_first = lhs.begin();
//	//	auto lhs_last = lhs.end();
//
//	//	auto rhs_first = rhs.begin();
//	//	auto rhs_last = rhs.end();
//
//	//	col.reset(*lhs_first, *rhs_first);
//
//	//	for (; lhs_first != lhs_last; ++lhs_first, ++rhs_first)
//	//	{
//	//		col(*lhs_first, *rhs_first);
//	//		std::cout << col.lhs().matrix() << '\n';
//	//		std::cout << col.rhs().matrix() << '\n';
//	//		std::cout << '\n';
//	//	}
//
//	//	assert(lhs_first == rhs_last);
//	//}
//
//	//kanalysis::arithmetic::TwoHandsSideColFunctor<Matrix, discreture::Combinations<Index>> col(mx);
//	//discreture::Combinations temp(mx.cols(), mx.cols() / 2);
//	//col.reset(*temp.begin(), temp.get_n(), temp.get_k());
//
//	//{
//	//	DEBUG_TIME();
//	//	auto first = temp.begin();
//	//	auto last = temp.end();
//	//	for (; first != last; ++first)
//	//	{
//	//		col(*first);
//	//		std::cout << col.lhs().matrix() << '\n';
//	//		std::cout << col.rhs().matrix() << '\n';
//	//		std::cout << '\n';
//	//	}
//	//}
//
//	//auto a = []() { return Vector(5); };
//
//	//int variables = 5;
//	//int nth_order = 4;
//	//int processors = 2;
//
//	//int packages = nth_order * processors;
//	//std::vector<std::future<void>> futures;
//	//futures.reserve(packages);
//
//	//Functor f;
//	//FormulaFunctor ff;
//
//	//auto custom2 = [&](discreture::Combinations<int>::iterator first1, discreture::Combinations<int>::iterator last1,
//	//				   discreture::Combinations<int>::reverse_iterator first2, discreture::Combinations<int>::reverse_iterator last2,
//	//				   int worker_id)
//	//{
//	//	ff(f(first1, last1, first2, last2, worker_id));
//
//	//	auto rfirst = std::make_reverse_iterator(first1);
//	//};
//
//	//threads::ThreadPool pool(processors);
//	//pool.start();
//
//	//for (int r = 4; r > 0; --r)
//	//{
//	//	auto lhs_combinatons = discreture::combinations(variables, r);
//	//	auto rhs_combination = discreture::reversed(discreture::combinations(variables, variables - r));
//
//	//	auto lhs_works = discreture::divide_work_in_equal_parts(lhs_combinatons, processors);
//	//	auto rhs_works = discreture::divide_work_in_equal_parts(rhs_combination, processors);
//
//	//	for (int i = 0; i < processors; ++i)
//	//	{
//	//		futures.emplace_back(pool.submit(custom2,
//	//										 lhs_works[i], lhs_works[i + 1],
//	//										 rhs_works[i], rhs_works[i + 1],
//	//										 std::placeholders::_1));
//	//	}
//	//	std::system("pause");
//	//}
//
//	//for (auto& f : futures)
//	//{
//	//	f.get();
//	//}
//
//	//pool.shutdown();
//
//	std::system("pause");
//}
//
////void test()
////{
////	//using namespace kanalysis::arithmetic;
////	using namespace kanalysis::stats;
////	using namespace kanalysis::tests;
////
////	int threads = 8;
////
////	{
////		int variables = 5;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_1.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			// Correct results: 0.320966, 0.190873, 0.209141, 0.037046, 0.241973
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			// Correct results: 0.30766828 0.19547034 0.20683219 0.05496811 0.23506109
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 10;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_2.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 15;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_2.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 18;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_2.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 20;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_3.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 25;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_3.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////
////	{
////		int variables = 30;
////		int threads = 4;
////
////		std::string filename = DATA_DIRECTORY;
////		filename += "test_data_3.csv";
////		Data data(filename, variables);
////
////		Matrix model_matrix = data.model_matrix();
////		Vector y = data.y();
////
////		std::cout << "Number of Variables: " << variables << '\n';
////		std::cout << "Number of Threads: " << threads << '\n';
////		std::cout << "Unweighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			Decomposition<Matrix, Array> c(model_matrix);
////			Kruskal<Decomposition<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////
////		std::cout << "Weighted:" << '\n';
////		{
////			DEBUG_TIME();
////
////			DecompositionWeight<Matrix, Array> c(model_matrix, data.weights());
////			KruskalWeight<DecompositionWeight<Matrix, Array>, LinearRegressionFunction> k(c);
////			std::cout << k.solve(y, 4) << '\n';
////		}
////	}
////	std::cout << "\n\n";
////	std::system("pause");
////}
//
////template<typename DerivedA, typename DerivedB>
////Scalar test(const MatrixBase<DerivedA>& mx, const VectorBase<DerivedB>& y)
////{
////	Decomposition<Matrix> qr(mx.derived());
////	Vector x_hat = qr.solve(y.derived());
////	Matrix ye = qr.matrixQR().triangularView<Eigen::Upper>();
////	Vector res = (qr.householderQ() * ye * x_hat);
////	return res.sum();
////}
////
////template<typename DerivedA, typename DerivedB>
////Scalar test2(const MatrixBase<DerivedA>& mxx, const VectorBase<DerivedB>& y)
////{
////	Matrix mx = mxx.derived();
////	Decomposition<Matrix> qr(mx);
////	Vector x_hat = qr.solve(y.derived());
////	Vector res = (mx * x_hat);
////	return res.sum();
////}
//
////template<typename DerivedA, typename DerivedB, typename DerivedC>
////Scalar CorrelationWeight::cor(const VectorBase<DerivedA>& fitted_values, const VectorBase<DerivedB>& residuals, const VectorBase<DerivedC>& weights) const
////{
////	Scalar sum_weights = weights.sum();
////
////	Vector weighted_fitted_values = weights.derived().array() * fitted_values.derived().array() / sum_weights;
////	Scalar mean = weighted_fitted_values.sum();
////
////	Scalar mss = (weights.derived().array() - fitted_values.derived().array() - mean).array().square().sum();
////	Scalar rss = (weights.derived().array() * residuals.array().square()).sum();
////
////	return mss / (mss + rss);
////}