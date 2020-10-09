#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"
#include "include/stats/weight_function.h"

namespace kanalysis::stats
{
	///
	/// \brief A wrapper class for keeping the model matrix and it's decomposition in sync.
	///
	/// \details The difference between this class and \a ComputeHolder is that all data stored in here is weighted.
	///
	template<typename MatrixType, typename ArrayType>
	class ComputeHolderWeight : public ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>
	{
	protected:
		using Base = ComputeHolderBase<ComputeHolderWeight<MatrixType, ArrayType>>;
	public:
		using Base::Base;
		ComputeHolderWeight() = default;

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB>
		ComputeHolderWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB, typename DerivedC>
		ComputeHolderWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights);

		template<typename DerivedA, typename DerivedB>
		void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const;

		template<typename Derived>
		Matrix standardize(const DenseBase<Derived>& x) const;

		template<typename Derived>
		ComputeHolderWeight<Derived, ArrayType> compute_holder(const DenseBase<Derived>& std_x) const;

		const ArrayType& weights() const;
		ArrayType& const_cast_weights() const;

		const Array& sqrt_weights() const;
		Array& const_cast_sqrt_weights() const;
	private:
		ArrayType m_weights;
		Array m_sqrt_weights = WeightFunction::sqrt_weights(m_weights);
	};

	template<typename MatrixType_, typename ArrayType_>
	struct ComputeHolderTraits<ComputeHolderWeight<MatrixType_, ArrayType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = ArrayType_;
	};

	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<DerivedA, DerivedB> compute_holder(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A constructor.
	///
	/// \details If \a weights is filled with a constant value of 1,
	/// then it is the same as using the regular \a ComputeHolder .
	///
	/// \details \a std_x and \a weights must have the same number of rows.
	///
	/// \param std_x A matrix with standardized values.
	/// \param weights An array of weights.
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
		: Base(std_x)
		, m_weights(weights.derived())
	{}

	///
	/// \brief A constructor for when \a MatrixType and \a ArrayType are a non-const reference type.
	///
	/// \overload ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights)
		: Base(std_x)
		, m_weights(weights.derived())
	{}

	///
	/// \internal
	///
	/// \sa ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights, const DenseBase<DerivedC>& sqrt_weights)
		: Base(std_x)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	///
	/// \internal
	///
	/// \sa ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights)
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB, typename DerivedC>
	ComputeHolderWeight<MatrixType, ArrayType>::ComputeHolderWeight(DenseBase<DerivedA>& std_x, DenseBase<DerivedB>& weights, DenseBase<DerivedC>& sqrt_weights)
		: Base(std_x)
		, m_weights(weights.derived())
		, m_sqrt_weights(sqrt_weights.derived())
	{}

	///
	/// \brief A conversion function that will standardize the values stored in \a x .
	/// The standardized values can be used for all other objects within the \a stats namespace.
	///
	/// \param x A matrix or vector.
	/// \param out The standardized values are outputted here.
	///
	template<typename MatrixType, typename ArrayType>
	template<typename DerivedA, typename DerivedB>
	void ComputeHolderWeight<MatrixType, ArrayType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out) const
	{
		WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights, out);
	}

	///
	/// \brief A conversion function that will standardize the values stored in \a x .
	/// The standardized values can be used for all other objects within the \a stats namespace.
	///
	/// \param x A matrix or vector.
	/// \return A matrix with standardized values.
	///
	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	Matrix ComputeHolderWeight<MatrixType, ArrayType>::standardize(const DenseBase<Derived>& x) const
	{
		return WeightFunction::multiply_by_sqrt_weights(x, m_sqrt_weights);
	}

	///
	/// \brief A factory function.
	///
	/// \param std_x A matrix or vector.
	/// \return A new \a ComputeHolderWeight where \a MatrixType is equal to \a std_x type, and \a ArrayType remains the same.
	///
	template<typename MatrixType, typename ArrayType>
	template<typename Derived>
	ComputeHolderWeight<Derived, ArrayType> ComputeHolderWeight<MatrixType, ArrayType>::compute_holder(const DenseBase<Derived>& std_x) const
	{
		return ComputeHolderWeight<Derived, ArrayType>(std_x, m_weights, m_sqrt_weights);
	}

	///
	/// \return The \a weights array.
	///
	template<typename MatrixType, typename ArrayType>
	const ArrayType& ComputeHolderWeight<MatrixType, ArrayType>::weights() const
	{
		return m_weights;
	}

	///
	/// \return The const casted \a weights array.
	///
	template<typename MatrixType, typename ArrayType>
	ArrayType& ComputeHolderWeight<MatrixType, ArrayType>::const_cast_weights() const
	{
		return const_cast<ArrayType&>(m_weights);
	}

	///
	/// \return The \a sqrt_weights array.
	///
	template<typename MatrixType, typename ArrayType>
	const Array& ComputeHolderWeight<MatrixType, ArrayType>::sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	///
	/// \return The const casted \a sqrt_weights array.
	///
	template<typename MatrixType, typename ArrayType>
	Array& ComputeHolderWeight<MatrixType, ArrayType>::const_cast_sqrt_weights() const
	{
		return m_sqrt_weights;
	}

	///
	/// \brief A global factory function.
	///
	/// \param std_x A matrix or vector.
	/// \param weights An array.
	/// \return A new \a ComputeHolderWeight where \a MatrixType is equal to \a std_x type, and \a ArrayType is equal to \a weights .
	///
	template<typename DerivedA, typename DerivedB>
	ComputeHolderWeight<DerivedA, DerivedB> compute_holder(const DenseBase<DerivedA>& std_x, const DenseBase<DerivedB>& weights)
	{
		return ComputeHolderWeight<DerivedA, DerivedB>(std_x, weights);
	}
} // namespace kanalysis::stats
