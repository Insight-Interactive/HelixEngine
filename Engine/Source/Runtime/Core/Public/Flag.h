#pragma once

#include "CoreFwd.h"


class CORE_API FFlag
{
public:
	FORCEINLINE FFlag()
		: m_FlagValue( false )
	{
	}
	FORCEINLINE ~FFlag()
	{
		Clear();
	}

	/*
		Returns true if the flag is set, false if not.
	*/
	FORCEINLINE bool IsSet() const
	{
		return m_FlagValue;
	}

	/*
		Sets the flag state to true.
	*/
	FORCEINLINE void Set()
	{
		m_FlagValue = true;
	}

	/*
		Sets the flag state to false.
	*/
	FORCEINLINE void Clear()
	{
		m_FlagValue = false;
	}

private:
	bool m_FlagValue;

};
