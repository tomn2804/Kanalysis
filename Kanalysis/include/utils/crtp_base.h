#pragma once

#include "include/config.h"

namespace kanalysis::utils
{
	template<typename DerivedType>
	class CrtpBase
	{
	public:
		DerivedType& derived();
		const DerivedType& derived() const;
		const DerivedType& const_derived() const;
	protected:
		CrtpBase() = default;
	};
} // namespace kanalysis::utils

namespace kanalysis::utils
{
	template<typename DerivedType>
	DerivedType& CrtpBase<DerivedType>::derived()
	{
		return static_cast<DerivedType&>(*this);
	}

	template<typename DerivedType>
	const DerivedType& CrtpBase<DerivedType>::derived() const
	{
		return static_cast<const DerivedType&>(*this);
	}

	template<typename DerivedType>
	const DerivedType& CrtpBase<DerivedType>::const_derived() const
	{
		return static_cast<const DerivedType&>(*this);
	}
} // namespace kanalysis::utils
