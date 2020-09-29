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
		SolveHolderBase(const ComputeHolderBase<Derived>& compute_holder);

		template<typename Derived>
		SolveHolderBase(ComputeHolderBase<Derived>& compute_holder);

		template<typename Derived>
		void compute(const DenseBase<Derived>& matrix);

		template<typename Derived>
		void standardized_compute(const DenseBase<Derived>& standardized_matrix);

		template<typename Derived>
		decltype(auto) solve(const VectorBase<Derived>& y);

		template<typename Derived>
		decltype(auto) standardized_solve(const VectorBase<Derived>& standardized_y);

		const auto& results() const;
		auto& const_cast_results() const;

		Index rows() const;
		Index cols() const;

		const MatrixType& standardized_matrix() const;
		MatrixType& const_cast_standardized_matrix() const;

		const Decomposition<Matrix>& decomposition() const;
		Decomposition<Matrix>& const_cast_decomposition() const;

		const ArrayType& weights() const;
		ArrayType& const_cast_weights() const;

		const Array& sqrt_weights() const;
		Array& const_cast_sqrt_weights() const;

		const ComputeHolderType& compute_holder() const;
		ComputeHolderType& const_cast_computer_holder() const;
	protected:
		SolveHolderBase() = default;
	private:
		ComputeHolderType m_compute_holder;
	};
} // namespace kanalysis::stats

namespace kanalysis::stats
{
	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(const ComputeHolderBase<Derived>& compute_holder)
		: m_compute_holder(compute_holder.derived())
	{}

	template<typename DerivedType>
	template<typename Derived>
	SolveHolderBase<DerivedType>::SolveHolderBase(ComputeHolderBase<Derived>& compute_holder)
		: m_compute_holder(compute_holder.derived())
	{}

	template<typename DerivedType>
	template<typename Derived>
	void SolveHolderBase<DerivedType>::compute(const DenseBase<Derived>& matrix)
	{
		m_compute_holder.compute(matrix);
	}

	template<typename DerivedType>
	template<typename Derived>
	void SolveHolderBase<DerivedType>::standardized_compute(const DenseBase<Derived>& standardized_matrix)
	{
		m_compute_holder.standardized_compute(standardized_matrix);
	}

	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) SolveHolderBase<DerivedType>::solve(const VectorBase<Derived>& y)
	{
		return Base::derived().solve(y);
	}
	template<typename DerivedType>
	template<typename Derived>
	decltype(auto) SolveHolderBase<DerivedType>::standardized_solve(const VectorBase<Derived>& standardized_y)
	{
		return Base::derived().standardized_solve(standardized_y);
	}

	template<typename DerivedType>
	const auto& SolveHolderBase<DerivedType>::results() const
	{
		return Base::derived().m_results;
	}

	template<typename DerivedType>
	auto& SolveHolderBase<DerivedType>::const_cast_results() const
	{
		return const_cast<auto&>(Base::derived().m_results);
	}

	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::rows() const
	{
		return m_compute_holder.rows();
	}

	template<typename DerivedType>
	Index SolveHolderBase<DerivedType>::cols() const
	{
		return m_compute_holder.cols();
	}

	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::standardized_matrix() const
	{
		return m_compute_holder.standardized_matrix();
	}

	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::MatrixType& SolveHolderBase<DerivedType>::const_cast_standardized_matrix() const
	{
		return m_compute_holder.const_cast_standardized_matrix();
	}

	template<typename DerivedType>
	const Decomposition<Matrix>& SolveHolderBase<DerivedType>::decomposition() const
	{
		return m_compute_holder.decomposition();
	}

	template<typename DerivedType>
	Decomposition<Matrix>& SolveHolderBase<DerivedType>::const_cast_decomposition() const
	{
		return m_compute_holder.const_cast_decomposition();
	}

	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::ArrayType& SolveHolderBase<DerivedType>::weights() const
	{
		return m_compute_holder.weights();
	}

	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::ArrayType& SolveHolderBase<DerivedType>::const_cast_weights() const
	{
		return m_compute_holder.const_cast_weights();
	}

	template<typename DerivedType>
	const Array& SolveHolderBase<DerivedType>::sqrt_weights() const
	{
		return m_compute_holder.sqrt_weights();
	}

	template<typename DerivedType>
	Array& SolveHolderBase<DerivedType>::const_cast_sqrt_weights() const
	{
		return m_compute_holder.const_cast_sqrt_weights();
	}

	template<typename DerivedType>
	const typename SolveHolderBase<DerivedType>::ComputeHolderType& SolveHolderBase<DerivedType>::compute_holder() const
	{
		return m_compute_holder;
	}

	template<typename DerivedType>
	typename SolveHolderBase<DerivedType>::ComputeHolderType& SolveHolderBase<DerivedType>::const_cast_computer_holder() const
	{
		return const_cast<ComputeHolderType&>(m_compute_holder);
	}
} // namespace kanalysis::stats