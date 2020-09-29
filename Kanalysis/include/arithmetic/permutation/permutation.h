#pragma once

#include "include/config.h"

#include "include/arithmetic/combination/combination.h"

namespace kanalysis::arithmetic::permutation
{
	namespace detail
	{
		void validate(int n);
		void validate(int n, int r);
		void validate(int n, int r, UInt k);
		void validate(int n, int r, UInt k, UInt p);
	} // namespace detail

	UInt factorial(int n);
	UInt factorial(int n, int r);
} // namespace kanalysis::arithmetic::permutation

namespace kanalysis::arithmetic::permutation
{
	namespace detail
	{
		KANALYSIS_INLINE void validate(int n)
		{
			combination::detail::validate(n);
		}

		KANALYSIS_INLINE void validate(int n, int r)
		{
			combination::detail::validate(n, r);
		}

		KANALYSIS_INLINE void validate(int n, int r, UInt k)
		{
			combination::detail::validate(n, r, k);
		}

		KANALYSIS_INLINE void validate(int n, int r, UInt k, UInt c)
		{
			combination::detail::validate(n, r, k);
			assert(c > 0);
			assert(c <= factorial(n, r));
			assert(k < c);
		}
	} // namespace detail

	KANALYSIS_INLINE UInt factorial(int n)
	{
		detail::validate(n);

		UInt result = 1;
		for (; n > 1; --n)
		{
			result *= n;
		}
		return result;
	}

	KANALYSIS_INLINE UInt factorial(int n, int r)
	{
		detail::validate(n, r);

		UInt result = 1;
		for (int k = 0; k < r; ++k)
		{
			result *= (n - k);
		}
		return result;
	}
} // namespace kanalysis::arithmetic::permutation