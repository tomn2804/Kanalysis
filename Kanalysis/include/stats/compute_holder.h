#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"

namespace kanalysis::stats
{
	///
	/// \brief A wrapper class for a decomposition from \a the Eigen library.
	///
	template<typename MatrixType>
	class ComputeHolder : public ComputeHolderBase<ComputeHolder<MatrixType>>
	{
	protected:
		using Base = ComputeHolderBase<ComputeHolder<MatrixType>>;
	public:
		using Base::Base;
		ComputeHolder() = default;

		template<typename DerivedA, typename DerivedB>
		static void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Matrix standardize(const DenseBase<Derived>& x);

		template<typename Derived>
		static ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_x);
	};

	///
	/// \brief A traits class for \a ComputeHolder
	///
	template<typename MatrixType_>
	struct ComputeHolderTraits<ComputeHolder<MatrixType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = Array;
	};

	template<typename Derived>
	ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_x);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \overload void ComputeHolderBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	///
	template<typename MatrixType>
	template<typename DerivedA, typename DerivedB>
	void ComputeHolder<MatrixType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		out.derived() = x.derived();
	}

	///
	/// \overload Matrix ComputeHolderBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	///
	template<typename MatrixType>
	template<typename Derived>
	Matrix ComputeHolder<MatrixType>::standardize(const DenseBase<Derived>& x)
	{
		return x.derived();
	}

	///
	/// \overload decltype(auto) ComputeHolderBase<DerivedType>::compute_holder(const DenseBase<Derived>& std_x)
	///
	template<typename MatrixType>
	template<typename Derived>
	ComputeHolder<Derived> ComputeHolder<MatrixType>::compute_holder(const DenseBase<Derived>& std_x)
	{
		return ComputeHolder<Derived>(std_x);
	}

	///
	/// \brief A global factory function.
	///
	/// \param std_x A matrix with standardized values.
	/// \return A new \a ComputeHolder where \a MatrixType is equal to \a std_x type.
	///
	template<typename Derived>
	ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_x)
	{
		return ComputeHolder<Derived>(std_x);
	}
} // namespace kanalysis::stats
