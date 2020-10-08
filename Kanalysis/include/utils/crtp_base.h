#pragma once

#include "include/config.h"

namespace kanalysis::utils
{
	///
	/// \brief A base class for all Curiously Recurring Template Pattern (CRTP) design.
	///
	template<typename DerivedType>
	class CrtpBase
	{
	public:
		DerivedType& derived();
		const DerivedType& derived() const;
		const DerivedType& const_derived() const;
		DerivedType& const_cast_derived() const;
	protected:
		CrtpBase() = default;
	};
} // namespace kanalysis::utils

namespace kanalysis::utils
{
	///
	/// \brief Returns the \a DerivedType .
	///
	/// \return The \a DerivedType .
	///
	template<typename DerivedType>
	DerivedType& CrtpBase<DerivedType>::derived()
	{
		return static_cast<DerivedType&>(*this);
	}

	///
	/// \overload CrtpBase<DerivedType>::derived()
	///
	template<typename DerivedType>
	const DerivedType& CrtpBase<DerivedType>::derived() const
	{
		return static_cast<const DerivedType&>(*this);
	}

	///
	/// \brief Returns the \a DerivedType as const.
	///
	/// \return The \a DerivedType as const.
	///
	template<typename DerivedType>
	const DerivedType& CrtpBase<DerivedType>::const_derived() const
	{
		return static_cast<const DerivedType&>(*this);
	}

	///
	/// \brief Returns the const casted of \a DerivedType .
	///
	/// \return The const casted of \a DerivedType .
	///
	template<typename DerivedType>
	DerivedType& CrtpBase<DerivedType>::const_cast_derived() const
	{
		return static_cast<DerivedType&>(const_cast<CrtpBase&>(*this));
	}
} // namespace kanalysis::utils
