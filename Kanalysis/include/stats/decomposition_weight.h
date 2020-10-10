#pragma once

#include "include/config.h"

#include "include/stats/decomposition_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	///
	/// \brief A wrapper class for a qr from \a the Eigen library.
	///
	/// \details The difference between this class and \a Decomposition is that all data stored in here is weighted.
	///
	template<typename MatrixType, typename ArrayType>
	class DecompositionWeight : public DecompositionBase<DecompositionWeight<MatrixType, ArrayType>>
	{
	protected:
		using Base = DecompositionBase<DecompositionWeight<MatrixType, ArrayType>>;
	public:
		using Base::Base;
		DecompositionWeight() = default;

		template<typename DerivedA, typename DerivedB>
		DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		DecompositionWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		DecompositionWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB>
		void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const;

		template<typename Derived>
		Matrix standardize(const DenseBase<Derived>& x) const;

		template<typename Derived>
		DecompositionWeight<Derived, ArrayType> decomposition(const DenseBase<Derived>& std_x) const;

		const ArrayType& weights() const;
		ArrayType& const_cast_weights() const;

		const Array& sqrt_weights() const;
		Array& const_cast_sqrt_weights() const;
	private:
		ArrayType m_weights;
		Array m_sqrt_weights = WeightFunction::sqrt_weights(m_weights);
	};

	template<typename MatrixType_, typename ArrayType_>
	struct QrDecompositionTraits<DecompositionWeight<MatrixType_, ArrayType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = ArrayType_;
	};

	template<typename DerivedA, typename DerivedB>
	DecompositionWeight<DerivedA, DerivedB> decomposition(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A constructor.
	///
	/// \details If \a weights is filled with a constant value of 1,
	/// then it is the same as using the regular \a Decomposition .
	///
	/// \details \a std_x and \a weights must have the same number of rows.
	///
	/// \param std_x A matrix with standardized values.
	/// \param weights An array of weights.
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
		: Base(std_x)
		, m_weights(weights.derived())
	{}

	///
	/// \overload DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights)
		: Base(std_x)
		, m_weights(weights.derived())
	{}

	///
	/// \internal
	///
	/// \sa DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights)
		: Base(std_x)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	///
	/// \internal
	///
	/// \sa DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	DecompositionWeight<MatrixType, ArrayType>::DecompositionWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights)
		: Base(std_x)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	///
	/// \overload void DecompositionBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	void DecompositionWeight<MatrixType, ArrayType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const
	{
		WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights, out);
	}

	///
	/// \overload Matrix DecompositionBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	Matrix DecompositionWeight<MatrixType, ArrayType>::standardize(const DenseBase<Derived>& x) const
	{
		return WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights);
	}

	///
	/// \overload decltype(auto) DecompositionBase<DerivedType>::decomposition(const DenseBase<Derived>& std_x)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	DecompositionWeight<Derived, ArrayType> DecompositionWeight<MatrixType, ArrayType>::decomposition(const DenseBase<Derived>& std_x) const
	{
		return DecompositionWeight<Derived, ArrayType>(std_x, m_weights, m_sqrt_weights);
	}

	///
	/// \return The \a weights array.
	///
	template<typename MatrixType, typename ArrayType>
	const ArrayType& DecompositionWeight<MatrixType, ArrayType>::weights() const
	{
		return m_weights;
	}

	///
	/// \return The const casted \a weights array.
	///
	template<typename MatrixType, typename ArrayType>
	ArrayType& DecompositionWeight<MatrixType, ArrayType>::const_cast_weights() const
	{
		return const_cast<ArrayType&>(m_weights);
	}

	///
	/// \return The \a sqrt_weights array.
	///
	template<typename MatrixType, typename ArrayType>
	const Array& DecompositionWeight<MatrixType, ArrayType>::sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	///
	/// \return The const casted \a sqrt_weights array.
	///
	template<typename MatrixType, typename ArrayType>
	Array& DecompositionWeight<MatrixType, ArrayType>::const_cast_sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	///
	/// \brief A global factory function.
	///
	/// \param std_x A matrix with standardized values.
	/// \param weights An array.
	/// \return A new \a DecompositionWeight where \a MatrixType is equal to \a std_x type, and \a ArrayType is equal to \a weights .
	///
	template<typename DerivedA, typename DerivedB>
	DecompositionWeight<DerivedA, DerivedB> decomposition(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	{
		return DecompositionWeight<DerivedA, DerivedB>(std_x, weights);
	}
} // namespace kanalysis::stats
