#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic
{
	///
	/// \brief A functor class for iterating over a matrix by columns.
	///
	/// \details This class is designed to be used with any \a for_each loop from the \a discreture library.
	///
	/// \details The idea behind this class is that for each iterations, the call operator is called and is given a vector of indices.
	/// In a corresponding order, this vector of indices is then used to match and re-arrange the columns of the underlying matrix.
	///
	/// \details The vector of indices can be a set of combinations, permutations, or multisets objects that are generated from the \a discreture library.
	///
	/// \details In addition, there are 2 underlying matrix in this object.
	/// The first matrix is the matrix to be mutated by \a reset() and is the one returned by the call operator. The type of this matrix is determined by \a MatrixType .
	/// The second matrix is the read-only matrix, that holds and provides the data to write to the first matrix. This matrix is constant and is only initialized once at constructor.
	///
	template<typename MatrixType>
	class ColFunctor
	{
	public:
		ColFunctor() = default;

		template<typename Derived>
		ColFunctor(const DenseBase<Derived>& matrix);

		template<typename Derived>
		ColFunctor(DenseBase<Derived>& matrix);

		ColFunctor(const ColFunctor& other);
		ColFunctor(ColFunctor&& other) noexcept;

		ColFunctor& operator=(const ColFunctor& rhs);
		ColFunctor& operator=(ColFunctor&& rhs) noexcept;

		template<typename Container>
		MatrixType operator()(const Container& x);

		template<typename Container>
		void reset(const Container& x);

		template<typename Derived>
		void relocate_matrix(const DenseBase<Derived>& matrix);

		template<typename Derived>
		void relocate_matrix(const Map<Derived>& matrix);

		const MatrixType& matrix() const;
		MatrixType& const_cast_matrix() const;

		const Matrix& read_only_matrix() const;
		Matrix& const_cast_read_only_matrix() const;
	private:
		void init();

		template<typename Container>
		Index find_middle_index(const Container& x);

		template<typename Derived>
		void resize_matrix(DenseBase<Derived>& matrix, Index cols);

		template<typename Derived>
		void resize_matrix(Map<Derived>& matrix, Index cols);

		MatrixType m_matrix;
		Matrix m_read_only_matrix = Matrix(m_matrix);

		std::vector<Scalar*> m_read_only_begin_col_ptrs = std::vector<Scalar*>(m_read_only_matrix.cols());
		std::vector<Scalar*> m_read_only_end_col_ptrs = std::vector<Scalar*>(m_read_only_matrix.cols());

		std::vector<Index> m_prev_indicies = std::vector<Index>(m_matrix.cols(), 0);
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	///
	/// \brief A constructor.
	///
	/// \details Choosing which indices or columns of the matrix to be iterated over is the task of the \a reset() function.
	///
	/// \param matrix A matrix to be iterated over.
	///
	template<typename MatrixType>
	template<typename Derived>
	ColFunctor<MatrixType>::ColFunctor(const DenseBase<Derived>& matrix)
		: m_matrix(matrix.derived())
	{
		init();
	}

	///
	/// \brief A constructor for when \a MatrixType is a non-const reference type.
	///
	/// \overload ColFunctor<MatrixType>::ColFunctor(const DenseBase<Derived>& matrix)
	///
	template<typename MatrixType>
	template<typename Derived>
	ColFunctor<MatrixType>::ColFunctor(DenseBase<Derived>& matrix)
		: m_matrix(matrix.derived())
	{
		init();
	}

	///
	/// \brief A copy constructor.
	///
	/// \param other A ColFunctor.
	///
	template<typename MatrixType>
	ColFunctor<MatrixType>::ColFunctor(const ColFunctor& other)
		: m_matrix(other.m_matrix)
		, m_read_only_matrix(other.m_read_only_matrix)
		, m_prev_indicies(other.m_prev_indicies)
	{
		init();
	}

	///
	/// \brief A move constructor.
	///
	/// \param other A ColFunctor.
	///
	template<typename MatrixType>
	ColFunctor<MatrixType>::ColFunctor(ColFunctor&& other) noexcept
		: m_matrix(std::move(other.m_matrix))
		, m_read_only_matrix(std::move(other.m_read_only_matrix))
		, m_prev_indicies(std::move(other.m_prev_indicies))
	{
		init();
	}

	///
	/// \brief A copy operator.
	///
	/// \param rhs A ColFunctor.
	///
	template<typename MatrixType>
	ColFunctor<MatrixType>& ColFunctor<MatrixType>::operator=(const ColFunctor& rhs)
	{
		if (&rhs == this) return *this;

		m_matrix = rhs.m_matrix;
		m_read_only_matrix = rhs.m_read_only_matrix;
		m_prev_indicies = rhs.m_prev_indicies;

		m_read_only_begin_col_ptrs = std::vector<Scalar*>(m_matrix.cols());
		m_read_only_end_col_ptrs = std::vector<Scalar*>(m_matrix.cols());
		init();

		return *this;
	}

	///
	/// \brief A move operator.
	///
	/// \param rhs A ColFunctor.
	///
	template<typename MatrixType>
	ColFunctor<MatrixType>& ColFunctor<MatrixType>::operator=(ColFunctor&& rhs) noexcept
	{
		if (&rhs == this) return *this;

		m_matrix = std::move(rhs.m_matrix);
		m_read_only_matrix = std::move(rhs.m_read_only_matrix);
		m_prev_indicies = std::move(rhs.m_prev_indicies);

		m_read_only_begin_col_ptrs = std::vector<Scalar*>(m_matrix.cols());
		m_read_only_end_col_ptrs = std::vector<Scalar*>(m_matrix.cols());
		init();

		return *this;
	}

	///
	/// \brief A call operator for each iterations generated of a \a for_each loop.
	///
	/// \warning \a reset() must be called before this call operator can be used.
	///
	/// \details Given a vector of indices from each iterations,
	/// this call operator will update the underlying matrix by columns to match with the corresponding indices.
	///
	/// \details The vector of indices must have \a x.size() equals to the number of columns of the underlying matrix.
	///
	/// \param x A vector of indices.
	/// \return A matrix with the corresponding columns.
	///
	template<typename MatrixType>
	template<typename Container>
	MatrixType ColFunctor<MatrixType>::operator()(const Container& x)
	{
		assert(x.size() == m_matrix.cols());
		assert(m_matrix.cols() == m_prev_indicies.size());

		assert(m_read_only_matrix.cols() == m_read_only_begin_col_ptrs.size());
		assert(m_read_only_begin_col_ptrs.size() == m_read_only_end_col_ptrs.size());

		Index middle = find_middle_index(x);
		if (middle != -1)
		{
			for (; middle < m_matrix.cols(); ++middle)
			{
				Index i = x[middle];
				if (i != m_prev_indicies[middle])
				{
					std::copy(m_read_only_begin_col_ptrs[i], m_read_only_end_col_ptrs[i], m_matrix.col(middle).data());
				}
			}
		}

		std::copy(x.begin(), x.end(), m_prev_indicies.begin());
		return m_matrix;
	}

	///
	/// \brief Set the initial state of the underlying matrix to match with the given the vector of indices \a x .
	///
	/// \details \a x.size() will determine how many columns there are on the matrix that is returned by the call operator.
	///
	/// \details The vector of indicies \a x must contain valid indices that are within the bound \a 0 to \code n_cols - 1 \endcode,
	/// where \a n_cols is the number of columns of the matrix given at constructor.
	///
	/// \param x A vector of indices.
	///
	template<typename MatrixType>
	template<typename Container>
	void ColFunctor<MatrixType>::reset(const Container& x)
	{
		resize_matrix(m_matrix, x.size());
		m_prev_indicies = std::vector<Index>(x.size(), 0);

		auto first = x.begin();
		for (Index i = 0; i < m_matrix.cols(); ++i, ++first)
		{
			std::copy(m_read_only_begin_col_ptrs[*first], m_read_only_end_col_ptrs[*first], m_matrix.col(i).data());
		}

		assert(first == x.end());
	}

	///
	/// \brief Reset the underlying matrix. Used incase of a matrix re-allocation occurs from the outside scope.
	///
	/// \details Matrix re-allocation occurs when the underlying matrix is being accessed from the outside,
	/// and it changed in a way that causes a new allocation to be made (e.g. \a resize() ).
	/// So changing the matrix from the outside invalidate the matrix in here, therefore \a relocate_matrix() is needed.
	///
	/// \param matrix A matrix to be iterated over.
	///
	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::relocate_matrix(const DenseBase<Derived>& matrix)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(matrix.cols() <= m_read_only_matrix.cols());
		m_matrix = matrix.derived();
	}

	///
	/// \overload void ColFunctor<MatrixType>::relocate_matrix(const DenseBase<Derived>& matrix)
	///
	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::relocate_matrix(const Map<Derived>& matrix)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(matrix.cols() <= m_read_only_matrix.cols());
		new (&m_matrix) Map<Derived>(matrix.derived().data(), matrix.rows(), matrix.cols());
	}

	///
	/// \brief Returns the underlying matrix.
	///
	/// \return The underlying matrix.
	///
	template<typename MatrixType>
	const MatrixType& ColFunctor<MatrixType>::matrix() const
	{
		return m_matrix;
	}

	///
	/// \brief Returns the const casted of the underlying matrix.
	///
	/// \return The const casted of the underlying matrix.
	///
	template<typename MatrixType>
	MatrixType& ColFunctor<MatrixType>::const_cast_matrix() const
	{
		return const_cast<MatrixType&>(m_matrix);
	}

	///
	/// \brief Returns the read-only matrix.
	///
	/// \return The read-only matrix.
	///
	template<typename MatrixType>
	const Matrix& ColFunctor<MatrixType>::read_only_matrix() const
	{
		return m_read_only_matrix;
	}

	///
	/// \brief Returns the const casted of the read-only matrix.
	///
	/// \return The const casted of the read-only matrix.
	///
	template<typename MatrixType>
	Matrix& ColFunctor<MatrixType>::const_cast_read_only_matrix() const
	{
		return const_cast<MatrixType&>(m_read_only_matrix);
	}

	///
	/// \brief Initialize function.
	///
	template<typename MatrixType>
	void ColFunctor<MatrixType>::init()
	{
		for (Index i = 0; i < m_read_only_matrix.cols(); ++i)
		{
			Scalar* read_only_col_ptr = m_read_only_matrix.col(i).data();
			m_read_only_begin_col_ptrs[i] = read_only_col_ptr;
			m_read_only_end_col_ptrs[i] = read_only_col_ptr + m_read_only_matrix.rows();
		}
	}

	///
	/// \brief Given a vector of indicies of a set of combiations or permutations or multisets,
	/// this function will find the starting middle point of where the index changes compared to the previous indicies.
	///
	/// \param x A vector of indicies.
	/// \return The middle index.
	///
	template<typename MatrixType>
	template<typename Container>
	Index ColFunctor<MatrixType>::find_middle_index(const Container& x)
	{
		assert(x.size() == m_prev_indicies.size());

		for (int i = 0; i < x.size(); ++i)
		{
			if (m_prev_indicies[i] != x[i]) return i;
		}
		return -1;
	}

	///
	/// \brief Resize the underlying matrix by columns.
	///
	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::resize_matrix(DenseBase<Derived>& matrix, Index cols)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(cols <= m_read_only_matrix.cols());
		matrix.derived().conservativeResize(matrix.rows(), cols);
	}

	///
	/// \overload void ColFunctor<MatrixType>::resize_matrix(DenseBase<Derived>& matrix, Index cols)
	///
	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::resize_matrix(Map<Derived>& matrix, Index cols)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(cols <= m_read_only_matrix.cols());
		new (&matrix.derived()) Map<Derived>(matrix.derived().data(), matrix.rows(), cols);
	}
} // namespace kanalysis::arithmetic
