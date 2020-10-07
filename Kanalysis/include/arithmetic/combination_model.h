#pragma once

#include "include/config.h"

#include "include/arithmetic/base_functions.h"

namespace kanalysis::arithmetic
{
	class CombinationModel
	{
	public:
		CombinationModel();
		CombinationModel(int n);
		CombinationModel(int n, int k);

		int n() const;
		int k() const;

		UInt c() const;
		UInt p() const;
		UInt binomial_sums() const;

		const std::vector<UInt>& multipliers() const;
	private:
		int m_n;
		int m_k;

		UInt m_c;
		UInt m_p;
		UInt m_binomial_sums;

		std::vector<UInt> m_multipliers;

		bool m_is_initialized;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	KANALYSIS_INLINE CombinationModel::CombinationModel()
		: m_is_initialized(false)
	{}

	KANALYSIS_INLINE CombinationModel::CombinationModel(int n, int k)
		: m_n(n)
		, m_k(k)
		, m_c(choose(n, k))
		, m_p(factorial(n))
		, m_binomial_sums(sums_of_binomial_coefficients(n, k))
		, m_multipliers(n)
		, m_is_initialized(true)
	{
		int crtl_variables = m_n - 1;

		m_multipliers[0] = m_p / m_n; // Multiplier value when nth_order is 0.

		UInt ctrl_permutations = factorial(crtl_variables);
		for (int i = 1; i < m_n; ++i)
		{
			m_multipliers[i] = ctrl_permutations / choose(crtl_variables, i);
		}
	}

	KANALYSIS_INLINE CombinationModel::CombinationModel(int n)
		: CombinationModel(n, n - 1)
	{}

	KANALYSIS_INLINE int CombinationModel::n() const
	{
		assert(m_is_initialized);
		return m_n;
	}

	KANALYSIS_INLINE int CombinationModel::k() const
	{
		assert(m_is_initialized);
		return m_k;
	}

	KANALYSIS_INLINE UInt CombinationModel::c() const
	{
		assert(m_is_initialized);
		return m_c;
	}

	KANALYSIS_INLINE UInt CombinationModel::p() const
	{
		assert(m_is_initialized);
		return m_p;
	}

	KANALYSIS_INLINE UInt CombinationModel::binomial_sums() const
	{
		assert(m_is_initialized);
		return m_binomial_sums;
	}

	KANALYSIS_INLINE const std::vector<UInt>& CombinationModel::multipliers() const
	{
		assert(m_is_initialized);
		return m_multipliers;
	}
} // namespace kanalysis::arithmetic
