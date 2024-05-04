// Copyright 2024 Insight Interactive. All Rights Reserved.
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
		// TODO This is not thread safe at all!
		m_FlagValue = true;
	}

	/*
		Sets the flag state to false.
	*/
	FORCEINLINE void Clear()
	{
		// TODO This is not thread safe at all!
		m_FlagValue = false;
	}

	FORCEINLINE void Wait()
	{
		while (m_FlagValue)
			std::this_thread::yield();
	}

private:
	bool m_FlagValue;

};
