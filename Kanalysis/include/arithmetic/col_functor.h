#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic
{
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
		void operator()(const Container& x);

		template<typename Container>
		void reset(const Container& x);

		template<typename Derived>
		void relocate_matrix(const DenseBase<Derived>& matrix);

		template<typename Derived>
		void relocate_matrix(const MapBase<Derived>& matrix);

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
		void resize_matrix(MapBase<Derived>& matrix, Index cols);

		MatrixType m_matrix;
		Matrix m_read_only_matrix = Matrix(m_matrix);

		std::vector<Scalar*> m_read_only_begin_col_ptrs = std::vector<Scalar*>(m_read_only_matrix.cols());
		std::vector<Scalar*> m_read_only_end_col_ptrs = std::vector<Scalar*>(m_read_only_matrix.cols());

		std::vector<Index> m_prev_indicies = std::vector<Index>(m_matrix.cols(), 0);
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	template<typename MatrixType>
	template<typename Derived>
	ColFunctor<MatrixType>::ColFunctor(const DenseBase<Derived>& matrix)
		: m_matrix(matrix.derived())
	{
		init();
	}

	template<typename MatrixType>
	template<typename Derived>
	ColFunctor<MatrixType>::ColFunctor(DenseBase<Derived>& matrix)
		: m_matrix(matrix.derived())
	{
		init();
	}

	template<typename MatrixType>
	ColFunctor<MatrixType>::ColFunctor(const ColFunctor& other)
		: m_matrix(other.m_matrix)
		, m_read_only_matrix(other.m_read_only_matrix)
		, m_prev_indicies(other.m_prev_indicies)
	{
		init();
	}

	template<typename MatrixType>
	ColFunctor<MatrixType>::ColFunctor(ColFunctor&& other) noexcept
		: m_matrix(std::move(other.m_matrix))
		, m_read_only_matrix(std::move(other.m_read_only_matrix))
		, m_prev_indicies(std::move(other.m_prev_indicies))
	{
		init();
	}

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

	template<typename MatrixType>
	template<typename Container>
	void ColFunctor<MatrixType>::operator()(const Container& x)
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
	}

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

	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::relocate_matrix(const DenseBase<Derived>& matrix)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(matrix.cols() <= m_read_only_matrix.cols());
		m_matrix = matrix.derived();
	}

	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::relocate_matrix(const MapBase<Derived>& matrix)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(matrix.cols() <= m_read_only_matrix.cols());
		using MapBaseType = typename std::decay<Derived>::type;
		new (&m_matrix) MapBaseType(matrix.derived().data(), matrix.rows(), matrix.cols());
	}

	template<typename MatrixType>
	const MatrixType& ColFunctor<MatrixType>::matrix() const
	{
		return m_matrix;
	}

	template<typename MatrixType>
	MatrixType& ColFunctor<MatrixType>::const_cast_matrix() const
	{
		return const_cast<MatrixType&>(m_matrix);
	}

	template<typename MatrixType>
	const Matrix& ColFunctor<MatrixType>::read_only_matrix() const
	{
		return m_read_only_matrix;
	}

	template<typename MatrixType>
	Matrix& ColFunctor<MatrixType>::const_cast_read_only_matrix() const
	{
		return const_cast<MatrixType&>(m_read_only_matrix);
	}

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

	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::resize_matrix(DenseBase<Derived>& matrix, Index cols)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(cols <= m_read_only_matrix.cols());
		matrix.derived().conservativeResize(matrix.rows(), cols);
	}

	template<typename MatrixType>
	template<typename Derived>
	void ColFunctor<MatrixType>::resize_matrix(MapBase<Derived>& matrix, Index cols)
	{
		assert(matrix.rows() == m_read_only_matrix.rows());
		assert(cols <= m_read_only_matrix.cols());
		using MapBaseType = typename std::decay<Derived>::type;
		new (&matrix.derived()) MapBaseType(matrix.derived().data(), matrix.rows(), cols);
	}
} // namespace kanalysis::arithmetic