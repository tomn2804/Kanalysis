#pragma once

#include "include/config.h"

#include "include/arithmetic/combination_iterator_base.h"

namespace kanalysis::arithmetic
{
	class CombinationIterator : public CombinationIteratorBase<CombinationIterator>
	{
	protected:
		using CombinationIteratorBase = CombinationIteratorBase<CombinationIterator>;
	public:
		using CombinationIteratorBase::CombinationIteratorBase;
	};
} // namespace kanalysis::arithmetic
