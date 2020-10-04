#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic
{
	UInt factorial(int n);
	UInt factorial(int n, int r);

	UInt choose(int n, int r);
	UInt cumulative_choose(int n, int r);
} // namespace kanalysis::arithmetic

namespace kanalysis::arithmetic
{
	KANALYSIS_INLINE UInt factorial(int n)
	{
		UInt result = 1;
		for (; n > 1; --n)
		{
			result *= n;
		}
		return result;
	}

	KANALYSIS_INLINE UInt factorial(int n, int r)
	{
		UInt result = 1;
		for (int k = 0; k < r; ++k)
		{
			result *= (n - k);
		}
		return result;
	}

	// https://stackoverflow.com/a/9331125
	KANALYSIS_INLINE UInt choose(int n, int r)
	{
		if (r > n) return 0;
		if (r * 2 > n) r = n - r;
		if (r == 0) return 1;

		UInt result = n;
		for (; r > 1; --r)
		{
			result *= (n - r + 1);
			result /= r;
		}
		return result;
	}

	KANALYSIS_INLINE UInt cumulative_choose(int n, int r)
	{
		UInt result = 0;
		for (; r > 0; --r)
		{
			result += choose(n, r);
		}
		return result;
	}
} // namespace kanalysis::arithmetic
