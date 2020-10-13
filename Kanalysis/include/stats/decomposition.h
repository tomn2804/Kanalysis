#pragma once

#include "config.h"

#include "stats/decomposition_base.h"

namespace kanalysis::stats
{
	///
	/// \brief A wrapper class for a qr from \a the Eigen library.
	///
	template<typename MatrixType>
	class Decomposition : public DecompositionBase<Decomposition<MatrixType>>
	{
	protected:
		using Base = DecompositionBase<Decomposition<MatrixType>>;
	public:
		using Base::Base;
		Decomposition() = default;

		template<typename DerivedA, typename DerivedB>
		static void standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out);

		template<typename Derived>
		static Matrix standardize(const DenseBase<Derived>& x);

		template<typename Derived>
		static Decomposition<Derived> decomposition(const DenseBase<Derived>& std_x);
	};

	///
	/// \brief A traits class for \a Decomposition
	///
	template<typename MatrixType_>
	struct QrDecompositionTraits<Decomposition<MatrixType_>>
	{
		using MatrixType = MatrixType_;
		using ArrayType = Array;
	};

	template<typename Derived>
	Decomposition<Derived> decomposition(const DenseBase<Derived>& std_x);
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \overload void DecompositionBase<DerivedType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	///
	template<typename MatrixType>
	template<typename DerivedA, typename DerivedB>
	void Decomposition<MatrixType>::standardize(const DenseBase<DerivedA>& x, DenseBase<DerivedB>& out)
	{
		out.derived() = x.derived();
	}

	///
	/// \overload Matrix DecompositionBase<DerivedType>::standardize(const DenseBase<Derived>& x)
	///
	template<typename MatrixType>
	template<typename Derived>
	Matrix Decomposition<MatrixType>::standardize(const DenseBase<Derived>& x)
	{
		return x.derived();
	}

	///
	/// \overload decltype(auto) DecompositionBase<DerivedType>::decomposition(const DenseBase<Derived>& std_x)
	///
	template<typename MatrixType>
	template<typename Derived>
	Decomposition<Derived> Decomposition<MatrixType>::decomposition(const DenseBase<Derived>& std_x)
	{
		return Decomposition<Derived>(std_x);
	}

	///
	/// \brief A global factory function.
	///
	/// \param std_x A matrix with standardized values.
	/// \return A new \a Decomposition where \a MatrixType is equal to \a std_x type.
	///
	template<typename Derived>
	Decomposition<Derived> decomposition(const DenseBase<Derived>& std_x)
	{
		return Decomposition<Derived>(std_x);
	}
} // namespace kanalysis::stats
