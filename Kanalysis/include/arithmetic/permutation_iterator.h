#pragma once

#include "include/config.h"

#include "include/arithmetic/permutation_iterator_base.h"

namespace kanalysis::arithmetic
{
	class PermutationIterator : public PermutationIteratorBase<PermutationIterator>
	{
	protected:
		using PermutationIteratorBase = PermutationIteratorBase<PermutationIterator>;
	public:
		using PermutationIteratorBase::PermutationIteratorBase;
	};
} // namespace kanalysis::arithmetic
