#pragma once

#include "include/config.h"

#include "include/stats/compute_holder_base.h"

namespace kanalysis::stats
{
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
		static ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_matrix);
	};

	template<typename MatrixType_>
	struct ComputeHolderTraits<ComputeHolder<MatrixType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = Array;
	};

	template<typename Derived>
	ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_matrix);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename MatrixType>
	template<typename DerivedA, typename DerivedB>
	void ComputeHolder<MatrixType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		out.derived() = x.derived();
	}

	template<typename MatrixType>
	template<typename Derived>
	Matrix ComputeHolder<MatrixType>::standardize(const DenseBase<Derived>& x)
	{
		return x.derived();
	}

	template<typename MatrixType>
	template<typename Derived>
	ComputeHolder<Derived> ComputeHolder<MatrixType>::compute_holder(const DenseBase<Derived>& std_matrix)
	{
		return ComputeHolder<Derived>(std_matrix);
	}

	template<typename Derived>
	ComputeHolder<Derived> compute_holder(const DenseBase<Derived>& std_matrix)
	{
		return ComputeHolder<Derived>(std_matrix);
	}
} // namespace kanalysis::stats
