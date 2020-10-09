#pragma once

#include "include/config.h"

#include "include/utils/crtp_base.h"

namespace kanalysis::stats
{
	template<typename DerivedType>
	struct SolveHolderTraits;

	template<typename DerivedType>
	struct ComputeHolderTraits;

	template<typename DerivedType>
	class ComputeHolderBase;

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
		using ComputeHolderType = typename SolveHolderTraits::ComputeHolderType;
		using ComputeHolderDecayType = typename std::decay<ComputeHolderType>::type;
		using ComputeHolderTraits = ComputeHolderTraits<ComputeHolderDecayType>;

		using MatrixType = typename ComputeHolderTraits::MatrixType;
		using ArrayType = typename ComputeHolderTraits::ArrayType;

		template<typename Derived>
		SolveHolderBase(const ComputeHolderBase<Derived>& decomposition);

		template<typename Derived>
		SolveHolderBase(ComputeHolderBase<Derived>& decomposition);

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

		const ComputeHolderType& decomposition() const;
		ComputeHolderType& const_cast_decomposition() const;
	protected:
		SolveHolderBase() = default;
	private:
		ComputeHolderType m_decomposition;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	///
	/// \brief A constructor.
	///
	/// \param decomposition A \a ComputeHolder or \a ComputeHolderWeight .
	///
	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(const ComputeHolderBase<Derived>& decomposition)
		: m_decomposition(decomposition.derived())
	{}

	///
	/// \overload SolveHolderBase<DerivedType>::SolveHolderBase(const ComputeHolderBase<Derived>& decomposition)
	///
	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(ComputeHolderBase<Derived>& decomposition)
		: m_decomposition(decomposition.derived())
	{}

	///
	/// \brief Compute the decomposition of \a std_x .
	///
	/// \param std_x A model matrix with standardized values.
	///
	template<typename DerivedType>
	template<typename Derived>
	void SolveHolderBase<DerivedType>::compute(const DenseBase<Derived>& std_x)
	{
		m_decomposition.compute(std_x);
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
	/// \return The number of rows of the underlying decomposition.
	///
	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::rows() const
	{
		return m_decomposition.rows();
	}

	///
	/// \return The number of columns of the underlying decomposition.
	///
	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::cols() const
	{
		return m_decomposition.cols();
	}

	///
	/// \return The matrix used in the underlying decomposition.
	///
	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::std_x() const
	{
		return m_decomposition.std_x();
	}

	///
	/// \return The const casted matrix used in the underlying decomposition.
	///
	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::const_cast_std_x() const
	{
		return m_decomposition.const_cast_std_x();
	}

	///
	/// \return The underlying decomposition.
	///
	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::ComputeHolderType& SolveHolderBase<DerivedType>::decomposition() const
	{
		return m_decomposition;
	}

	///
	/// \return The underlying decomposition.
	///
	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::ComputeHolderType& SolveHolderBase<DerivedType>::const_cast_decomposition() const
	{
		return const_cast<ComputeHolderType&>(m_decomposition);
	}
} // namespace kanalysis::stats
