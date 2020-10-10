#pragma once

#include "include/config.h"

#include "include/utils/crtp_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct SolveHolderTraits;

	template<typename DerivedType>
	struct QrDecompositionTraits;

	template<typename DerivedType>
	class DecompositionBase;

	///
	/// \brief A CRTP base class all solver classes found in the \a stats namespace.
	///
	template<typename DerivedType>
	class SolveHolderBase : public utils::CrtpBase<DerivedType>
	{
	protected:
		using Base = utils::CrtpBase<DerivedType>;
		using SolveHolderTraits = SolveHolderTraits<DerivedType>;
		using RegressionFunctionType = typename SolveHolderTraits::RegressionFunctionType;
	public:
		using DecompositionType = typename SolveHolderTraits::DecompositionType;
		using ComputeHolderDecayType = typename std::decay<DecompositionType>::type;
		using QrDecompositionTraits = QrDecompositionTraits<ComputeHolderDecayType>;

		using MatrixType = typename QrDecompositionTraits::MatrixType;
		using ArrayType = typename QrDecompositionTraits::ArrayType;

		template<typename Derived>
		SolveHolderBase(const DecompositionBase<Derived>& qr);

		template<typename Derived>
		SolveHolderBase(DecompositionBase<Derived>& qr);

		template<typename Derived>
		void compute(const DenseBase<Derived>& std_x);

		template<typename Derived>
		decltype(auto) solve(const VectorBase<Derived>& std_y) const;

		const auto& recent_results() const;
		auto& const_cast_recent_results() const;

		Index rows() const;
		Index cols() const;

		const MatrixType& std_x() const;
		MatrixType& const_cast_std_x() const;

		const DecompositionType& qr() const;
		DecompositionType& const_cast_qr() const;
	protected:
		SolveHolderBase() = default;
	private:
		DecompositionType m_qr;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A constructor.
	///
	/// \param qr A \a Decomposition or \a DecompositionWeight .
	///
	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(const DecompositionBase<Derived>& qr)
		: m_qr(qr.derived())
	{}

	///
	/// \overload SolveHolderBase<DerivedType>::SolveHolderBase(const DecompositionBase<Derived>& qr)
	///
	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(DecompositionBase<Derived>& qr)
		: m_qr(qr.derived())
	{}

	///
	/// \brief Compute the qr of \a std_x .
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	void SolveHolderBase<DerivedType>::compute(const DenseBase<Derived>& std_x)
	{
		m_qr.compute(std_x);
	}

	///
	/// \brief Dispatch function.
	///
	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) SolveHolderBase<DerivedType>::solve(const VectorBase<Derived>& std_y) const
	{
		return Base::derived().solve(std_y);
	}

	///
	/// \return The recent results after calling \a solve()
	///
	template<typename DerivedType>
	const auto& SolveHolderBase<DerivedType>::recent_results() const
	{
		return Base::derived().m_results;
	}

	///
	/// \return The const casted recent results after calling \a solve()
	///
	template<typename DerivedType>
	auto& SolveHolderBase<DerivedType>::const_cast_recent_results() const
	{
		return const_cast<decltype(Base::derived().m_results)>(Base::derived().m_results);
	}

	///
	/// \return The number of rows of the underlying qr.
	///
	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::rows() const
	{
		return m_qr.rows();
	}

	///
	/// \return The number of columns of the underlying qr.
	///
	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::cols() const
	{
		return m_qr.cols();
	}

	///
	/// \return The matrix used in the underlying qr.
	///
	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::std_x() const
	{
		return m_qr.std_x();
	}

	///
	/// \return The const casted matrix used in the underlying qr.
	///
	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::const_cast_std_x() const
	{
		return m_qr.const_cast_std_x();
	}

	///
	/// \return The underlying qr.
	///
	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::DecompositionType& SolveHolderBase<DerivedType>::qr() const
	{
		return m_qr;
	}

	///
	/// \return The underlying qr.
	///
	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::DecompositionType& SolveHolderBase<DerivedType>::const_cast_qr() const
	{
		return const_cast<DecompositionType&>(m_qr);
	}
} // namespace kanalysis::stats
