#pragma once

#include "DataTypes.h"

template <typename T> 
FORCEINLINE T AlignUpWithMask(T value, size_t mask)
{
	return (T)(((size_t)value + mask) & ~mask);
}

template <typename T> 
FORCEINLINE T AlignUp(T value, size_t alignment)
{
	return AlignUpWithMask(value, alignment - 1);
}

FORCEINLINE float FloatEqualsFloat( const float& Float1, const float& Float2, const float& Epsilon = 0.01f)
{
	return fabs( Float1 - Float2 ) < Epsilon;
}
