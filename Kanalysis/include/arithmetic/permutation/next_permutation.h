#pragma once

#include "include/config.h"

namespace kanalysis::arithmetic::permutation
{
	template<typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last);

	template<typename BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last);

	template<typename BidirectionalIterator>
	bool next_partial_permutation(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last);

	template<typename BidirectionalIterator>
	bool prev_partial_permutation(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last);
} // namespace kanalysis::arithmetic::permutation

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2639.pdf
namespace kanalysis::arithmetic::permutation
{
	template<typename BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		return std::next_permutation(first, last);
	}

	template<typename BidirectionalIterator>
	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		return std::prev_permutation(first, last);
	}

	template<typename BidirectionalIterator>
	bool next_partial_permutation(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
		std::reverse(middle, last);
		return std::next_permutation(first, last);
	}

	template<typename BidirectionalIterator>
	bool prev_partial_permutation(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
	{
		bool result = std::prev_permutation(first, last);
		std::reverse(middle, last);
		return result;
	}
} // namespace kanalysis::arithmetic::permutation