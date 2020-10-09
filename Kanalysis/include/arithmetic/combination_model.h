#pragma once

#include "include/config.h"

#include "include/arithmetic/base_functions.h"

namespace kanalysis::arithmetic
{
	///
	/// \brief A class to store the necessary components to calculate combinations.
	///
	class CombinationModel
	{
	public:
		CombinationModel() = default;
		CombinationModel(int n, int k);

		int n() const;
		int k() const;

		UInt c() const;
		UInt p() const;
		UInt binomial_sums() const;

		const std::vector<UInt>& multipliers() const;
	private:
		int m_n = 0;
		int m_k = 0;

		UInt m_c = choose(m_n, m_k);
		UInt m_p = factorial(m_n);
		UInt m_binomial_sums = sums_of_binomial_coefficients(m_n, m_k);

		std::vector<UInt> m_multipliers;
	};
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	///
	/// \brief A constructor.
	///
	/// \param n An interger.
	/// \param k An interger.
	///
	KANALYSIS_INLINE CombinationModel::CombinationModel(int n, int k)
		: m_n(n)
		, m_k(k)
		, m_multipliers(n)
	{
		int r = m_n - 1; // Number of controlled variables.

		m_multipliers[0] = m_p / m_n; // Multiplier value when nth_order is 0.

		UInt p = factorial(r);
		for (int i = 1; i < m_n; ++i)
		{
			m_multipliers[i] = p / choose(r, i); // Multiplier value when nth_order is i.
		}
	}

	///
	/// \return The value \a n .
	///
	KANALYSIS_INLINE int CombinationModel::n() const
	{
		return m_n;
	}

	///
	/// \return The value \a k .
	///
	KANALYSIS_INLINE int CombinationModel::k() const
	{
		return m_k;
	}

	///
	/// \return The value of \a n choose \a k .
	///
	KANALYSIS_INLINE UInt CombinationModel::c() const
	{
		return m_c;
	}

	///
	/// \return The value of \a factorial(n) .
	///
	KANALYSIS_INLINE UInt CombinationModel::p() const
	{
		return m_p;
	}

	///
	/// \return The value of \a sums_of_binomial_coefficients(n, k) .
	///
	KANALYSIS_INLINE UInt CombinationModel::binomial_sums() const
	{
		return m_binomial_sums;
	}

	///
	/// \brief To-do: Expand on this.
	///
	/// \return A vector of multipliers.
	///
	KANALYSIS_INLINE const std::vector<UInt>& CombinationModel::multipliers() const
	{
		return m_multipliers;
	}
} // namespace kanalysis::arithmetic
