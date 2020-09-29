#include "tests/tests.h"

#define ARMA_MAT_PREALLOC 1
#define ARMA_USE_BLAS
#define ARMA_USE_CXX11
#define ARMA_USE_LAPACK

#include <armadillo>
#include <iomanip>
#include <numeric>

#define MODEL_MATRIX_INTERCEPT_COL_INDEX 0
#define MODEL_MATRIX_INTERCEPT_COL_VALUE 1

#define DATA_WEIGHTS_COL_INDEX 0
#define DATA_X_START_COL_INDEX 2
#define DATA_Y_COL_INDEX 1

namespace kanalysis::tests
{
	Time::Time()
		: m_start(std::chrono::high_resolution_clock::now())
	{}

	Time::~Time()
	{
		std::cout << "Total Time Elapsed: " << *this << '\n';
	}

	const std::chrono::time_point<std::chrono::steady_clock>& Time::start()
	{
		return m_start;
	}

	// https://stackoverflow.com/a/46134506
	std::ostream& operator<<(std::ostream& os, const Time& rhs)
	{
		using namespace std::chrono;
		using days = duration<int, std::ratio<86400>>;

		auto stop = high_resolution_clock::now();
		auto elapsed = duration_cast<milliseconds>(stop - rhs.m_start);

		auto d = duration_cast<days>(elapsed);
		elapsed -= d;
		auto h = duration_cast<hours>(elapsed);
		elapsed -= h;
		auto m = duration_cast<minutes>(elapsed);
		elapsed -= m;
		auto s = duration_cast<seconds>(elapsed);
		elapsed -= s;
		auto ms = duration_cast<milliseconds>(elapsed);

		os.fill('0');
		os << std::setw(2) << d.count() << "d:";
		os << std::setw(2) << h.count() << "h:";
		os << std::setw(2) << m.count() << "m:";
		os << std::setw(2) << s.count() << "s:";
		os << std::setw(2) << ms.count() << "ms";
		return os;
	}

	Data::Data(const std::string& filename, int variables)
		: Data(filename, variables, variables - 1)
	{}

	Data::Data(const std::string& filename, int variables, int nth_order)
		: Data(filename, variables, nth_order, 1)
	{}

	Data::Data(const std::string& filename, int variables, int nth_order, int threads)
		: m_filename(filename)
		, m_variables(variables)
		, m_nth_order(nth_order)
		, m_threads(threads)
	{
		arma::Mat<Scalar> data;
		data.load(filename);

		arma::Col<arma::uword> row_indices(data.n_rows - 1);
		std::iota(row_indices.begin(), row_indices.end(), 1);
		data = data.rows(row_indices);

		arma::Mat<Scalar> model_matrix(data.colptr(DATA_X_START_COL_INDEX), data.n_rows, variables, false, true);
		arma::Col<Scalar> y(data.colptr(DATA_Y_COL_INDEX), data.n_rows, false, true);
		arma::Col<Scalar> weights(data.colptr(DATA_WEIGHTS_COL_INDEX), data.n_rows, false, true);

		m_model_matrix = Matrix(data.n_rows, variables + 1);
		m_y = Vector(data.n_rows);
		m_weights = Vector(data.n_rows);

		m_model_matrix.col(MODEL_MATRIX_INTERCEPT_COL_INDEX).fill(MODEL_MATRIX_INTERCEPT_COL_VALUE);
		std::copy(model_matrix.begin(), model_matrix.end(), m_model_matrix.col(MODEL_MATRIX_INTERCEPT_COL_INDEX + 1).data());
		std::copy(y.begin(), y.end(), m_y.data());
		std::copy(weights.begin(), weights.end(), m_weights.data());
	}

	const std::string& Data::filename() const
	{
		return m_filename;
	}

	int Data::variables() const
	{
		return m_variables;
	}

	int Data::nth_order() const
	{
		return m_nth_order;
	}

	int Data::threads() const
	{
		return m_threads;
	}

	const Matrix& Data::model_matrix() const
	{
		return m_model_matrix;
	}

	const Vector& Data::y() const
	{
		return m_y;
	}

	const Vector& Data::weights() const
	{
		return m_weights;
	}
} // namespace kanalysis::tests

//namespace kanalysis::arithmetic
//{
//	template<typename LhsMatrixType, typename ArithmeticType>
//	class TwoHandSideColFunctor
//	{
//	public:
//		TwoHandSideColFunctor() = default;
//
//		template<typename Derived>
//		TwoHandSideColFunctor(const DenseBase<Derived>& lhs_matrix);
//
//		template<typename Derived>
//		TwoHandSideColFunctor(DenseBase<Derived>& lhs_matrix);
//
//		template<typename Container>
//		void operator()(const Container& x);
//
//		template<typename Container>
//		void reset(const Container& x, int n, int k);
//
//		const ColFunctor<LhsMatrixType>& lhs() const;
//		ColFunctor<LhsMatrixType>& const_cast_lhs() const;
//
//		const ColFunctor<Matrix>& rhs() const;
//		ColFunctor<Matrix>& const_cast_rhs() const;
//	private:
//		ColFunctor<LhsMatrixType> m_lhs;
//		ColFunctor<Matrix> m_rhs;
//		typename discreture::Reversed<ArithmeticType>::iterator m_rhs_iterator;
//	};
//} // namespace kanalysis::arithmetic
//
//namespace kanalysis::arithmetic
//{
//	template<typename LhsMatrixType, typename ArithmeticType>
//	template<typename Derived>
//	TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::TwoHandSideColFunctor(const DenseBase<Derived>& lhs_matrix)
//		: m_lhs(lhs_matrix)
//		, m_rhs(lhs_matrix)
//	{}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	template<typename Derived>
//	TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::TwoHandSideColFunctor(DenseBase<Derived>& lhs_matrix)
//		: m_lhs(lhs_matrix)
//		, m_rhs(lhs_matrix)
//	{}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	template<typename Container>
//	void TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::operator()(const Container& x)
//	{
//		m_lhs(x);
//		m_rhs(*m_rhs_iterator);
//		++m_rhs_iterator;
//	}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	template<typename Container>
//	void TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::reset(const Container& x, int n, int k)
//	{
//		m_lhs.reset(x);
//		Index i = ArithmeticType::get_index(x);
//		m_rhs_iterator = discreture::reversed(ArithmeticType(n, (n - k))).begin() + i;
//		m_rhs.reset(*m_rhs_iterator);
//	}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	const ColFunctor<LhsMatrixType>& TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::lhs() const
//	{
//		return m_lhs;
//	}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	ColFunctor<LhsMatrixType>& TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::const_cast_lhs() const
//	{
//		return const_cast<ColFunctor<LhsMatrixType>&>(m_lhs);
//	}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	const ColFunctor<Matrix>& TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::rhs() const
//	{
//		return m_rhs;
//	}
//
//	template<typename LhsMatrixType, typename ArithmeticType>
//	ColFunctor<Matrix>& TwoHandSideColFunctor<LhsMatrixType, ArithmeticType>::const_cast_rhs() const
//	{
//		return const_cast<ColFunctor<Matrix>&>(m_rhs);
//	}
//} // namespace kanalysis::arithmetic