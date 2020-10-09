#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic
{
	UInt factorial(int n);
	UInt factorial(int n, int k);

	UInt choose(int n, int k);

	UInt sums_of_binomial_coefficients(int n);
	UInt sums_of_binomial_coefficients(int n, int k);
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	///
	/// \brief Calculate the factorial of \a n .
	///
	/// \details https://en.wikipedia.org/wiki/Factorial
	///
	/// \param n An interger.
	/// \return An unsigned interger.
	///
	KANALYSIS_INLINE UInt factorial(int n)
	{
		UInt result = 1;
		for (; n > 1; --n)
		{
			result *= n;
		}
		return result;
	}

	///
	/// \brief Calculate the factorial of \a n given \a k .
	///
	/// \details https://en.wikipedia.org/wiki/Factorial
	///
	/// \param n An interger.
	/// \param k An interger.
	/// \return An unsigned interger.
	///
	KANALYSIS_INLINE UInt factorial(int n, int k)
	{
		UInt result = 1;
		for (int r = 0; r < k; ++r)
		{
			result *= (n - r);
		}
		return result;
	}

	///
	/// \brief Calculate the binomial coefficient of \a n given \a k .
	///
	/// \details https://stackoverflow.com/a/9331125
	///
	/// \param n An interger.
	/// \param k An interger.
	/// \return An unsigned interger.
	///
	KANALYSIS_INLINE UInt choose(int n, int k)
	{
		if (k > n) return 0;
		if (k * 2 > n) k = n - k;
		if (k == 0) return 1;

		UInt result = n;
		for (; k > 1; --k)
		{
			result *= (n - k + 1);
			result /= k;
		}
		return result;
	}

	///
	/// \brief Calculate the sums of the binomial coefficients of \a n .
	///
	/// \details https://en.wikipedia.org/wiki/Binomial_coefficient
	///
	/// \param n An interger.
	/// \return An unsigned interger.
	///
	KANALYSIS_INLINE UInt sums_of_binomial_coefficients(int n)
	{
		return std::pow(2, n);
	}

	///
	/// \brief Calculate the sums of the binomial coefficients of \a n given \a k .
	///
	/// \details https://en.wikipedia.org/wiki/Binomial_coefficient
	///
	/// \param n An interger.
	/// \param k An interger.
	/// \return An unsigned interger.
	///
	KANALYSIS_INLINE UInt sums_of_binomial_coefficients(int n, int k)
	{
		UInt result = 0;
		for (; k >= 0; --k)
		{
			result += choose(n, k);
		}
		return result;
	}
} // namespace kanalysis::arithmetic
