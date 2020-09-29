#pragma once

#include "include/config.h"

#include "include/arithmetic/combination/combination.h"
#include "include/arithmetic/permutation/permutation.h"

namespace kanalysis::arithmetic
{
	class ForwardCombinationIterator;

	class CombinationModel
	{
	public:
		using Iterator = ForwardCombinationIterator;

		CombinationModel();
		CombinationModel(int n);
		CombinationModel(int n, int r);

		int n() const;
		int r() const;

		UInt c() const;
		UInt p() const;
		UInt cumulative_c() const;

		const std::vector<UInt>& multipliers() const;
	private:
		int m_n;
		int m_r;

		UInt m_c;
		UInt m_p;
		UInt m_cumulative_c;

		std::vector<UInt> m_multipliers;

		bool m_is_initialized;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	KANALYSIS_INLINE CombinationModel::CombinationModel()
		: m_is_initialized(false)
	{}

	KANALYSIS_INLINE CombinationModel::CombinationModel(int n, int r)
		: m_n(n)
		, m_r(r)
		, m_c(combination::choose(n, r))
		, m_p(permutation::factorial(n))
		, m_cumulative_c(combination::cumulative_choose(n, r))
		, m_multipliers(n)
		, m_is_initialized(true)
	{
		int crtl_variables = m_n - 1;

		m_multipliers[0] = m_p / m_n; // Multiplier value when nth_order is 0.

		UInt ctrl_permutations = permutation::factorial(crtl_variables);
		for (int i = 1; i < m_n; ++i)
		{
			m_multipliers[i] = ctrl_permutations / combination::choose(crtl_variables, i);
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

	KANALYSIS_INLINE int CombinationModel::r() const
	{
		assert(m_is_initialized);
		return m_r;
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

	KANALYSIS_INLINE UInt CombinationModel::cumulative_c() const
	{
		assert(m_is_initialized);
		return m_cumulative_c;
	}

	KANALYSIS_INLINE const std::vector<UInt>& CombinationModel::multipliers() const
	{
		assert(m_is_initialized);
		return m_multipliers;
	}
} // namespace kanalysis::arithmetic