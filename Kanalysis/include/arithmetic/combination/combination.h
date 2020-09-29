#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic::combination
{
	namespace detail
	{
		void validate(int n);
		void validate(int n, int r);
		void validate(int n, int r, UInt k);
		void validate(int n, int r, UInt k, UInt c);

		int largest_v(int a, int b, UInt x);

		template<typename OutputIterator>
		void combinadic(int n, int r, UInt k, OutputIterator d_first);
	} // namespace detail

	UInt choose(int n, int r);
	UInt cumulative_choose(int n, int r);

	template<typename OutputIterator>
	void combinations(int n, int r, UInt k, OutputIterator d_first);

	template<typename Int>
	std::vector<Int> combinations(int n, int r, UInt k);
} // namespace kanalysis::arithmetic::combination

// https://vlkan.com/blog/post/2013/12/04/combinations/
// https://github.com/vy/combination/blob/master/src/main/java/com/vlkan/combination/Combination.java
namespace kanalysis::arithmetic::combination
{
	namespace detail
	{
		KANALYSIS_INLINE void validate(int n)
		{
			assert(n >= 0);
		}

		KANALYSIS_INLINE void validate(int n, int r)
		{
			validate(n);
			assert(r >= 0);
			assert(r <= n);
		}

		KANALYSIS_INLINE void validate(int n, int r, UInt k)
		{
			validate(n, r);
			assert(k >= 0);
		}

		KANALYSIS_INLINE void validate(int n, int r, UInt k, UInt c)
		{
			validate(n, r, k);
			assert(c > 0);
			assert(c <= choose(n, r));
			assert(k < c);
		}

		KANALYSIS_INLINE int largest_v(int a, int b, UInt x)
		{
			int v = a - 1;
			while (choose(v, b) > x)
			{
				--v;
			}
			return v;
		}

		template<typename OutputIterator>
		void combinadic(int n, int r, UInt k, OutputIterator d_first)
		{
			for (; r > 0; --r, ++d_first)
			{
				*d_first = largest_v(n, r, k);
				k -= choose(*d_first, r);
				n = *d_first;
			}
		}
	} // namespace detail

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
		detail::validate(n, r);

		UInt result = 0;
		for (; r > 0; --r)
		{
			result += choose(n, r);
		}
		return result;
	}

	// E.g., 8th combination of 5C2 = combinations(5, 2, 8, out.begin()) = [2, 4]
	template<typename OutputIterator>
	void combinations(int n, int r, UInt k, OutputIterator d_first)
	{
		UInt c = choose(n, r);
		detail::validate(n, r, k, c);

		UInt j = c - k - 1;
		detail::combinadic(n, r, j, d_first);

		for (; r > 0; --r, ++d_first)
		{
			*d_first = n - *d_first - 1;
		}
	}

	template<typename Int>
	std::vector<Int> combinations(int n, int r, UInt k)
	{
		std::vector<Int> results(r);
		combinations(n, r, k, results.begin());
		return results;
	}
} // namespace kanalysis::arithmetic::combination