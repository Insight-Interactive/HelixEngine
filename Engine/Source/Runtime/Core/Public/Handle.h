#pragma once

#include "CoreFwd.h"

const uint32 kInvalidHandle = 0xFFFFFFFF;


class CORE_API Handle
{
public:
	Handle()
		: m_Handle(kInvalidHandle)
	{
	}
	Handle(uint32 Value)
		: m_Handle(Value)
	{
	}
	inline bool IsValid() const
	{
		return m_Handle != kInvalidHandle;
	}
	inline void InValidate()
	{
		m_Handle = kInvalidHandle;
	}

	Handle& operator++()
	{
		m_Handle++;
		return *this;
	}
	Handle operator++(int)
	{
		Handle Temp = *this;
		++(*this);
		return Temp;
	}
	Handle& operator--()
	{
		m_Handle--;
		return *this;
	}
	Handle operator--(int)
	{
		Handle Temp = *this;
		--(*this);
		return Temp;
	}
	Handle& operator=(const Handle& Other)
	{
		this->m_Handle = Other.m_Handle;
		return *this;
	}
	FORCEINLINE bool operator==(const Handle& Other)
	{
		return this->m_Handle == Other.m_Handle;
	}
	FORCEINLINE bool operator!=(const Handle& Other)
	{
		return !(this->m_Handle == Other.m_Handle);
	}
private:
	uint32 m_Handle;
};
