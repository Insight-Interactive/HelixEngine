// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "TArray.h"


#if HE_TARRAY_DEBUG_LEVEL	== 2
#	define HE_TARRAY_THROW_ON_OUT_OF_BOUNDS 1

#elif HE_TARRAY_DEBUG_LEVEL == 1
#	define HE_TARRAY_WARN_ON_OUT_OF_BOUNDS	1

#elif HE_TARRAY_DEBUG_LEVEL == 0
#	define HE_TARRY_THROW_ON_OUT_OF_BOUNDS	0
#	define HE_TARRAY_WARN_ON_OUT_OF_BOUNDS	0

#endif



template<typename ElementType, uint64 NumElements>
uint64 TArray<ElementType, NumElements>::Length()
{
	return NumElements;
}

template<typename ElementType, uint64 NumElements>
TArray<ElementType, NumElements>::TArray()
{
	ZeroMemory(m_Data, sizeof(m_Data));
}

template<typename ElementType, uint64 NumElements>
TArray<ElementType, NumElements>::~TArray()
{
}

template<typename ElementType, uint64 NumElements>
ElementType& TArray<ElementType, NumElements>::operator[](const uint64& Index)
{
	return m_Data[Index];
}

template<typename ElementType, uint64 NumElements>
ElementType& TArray<ElementType, NumElements>::At(const uint64& Index)
{
	if (Index >= NumElements) 
	{
#if HE_TARRAY_THROW_ON_OUT_OF_BOUNDS
		throw; // Index out of bounds. 
#endif
#if HE_TARRAY_WARN_ON_OUT_OF_BOUNDS 
		HE_LOG(Warning, TEXT("Array index {%u} out of bounds! Array max length=%u"), Index, Length());
#endif
	} 

	return m_Data[Index];
}

template<typename ElementType, uint64 NumElements>
ElementType* TArray<ElementType, NumElements>::Data()
{
	return m_Data;
}

template<typename ElementType, uint64 NumElements>
const ElementType* TArray<ElementType, NumElements>::DataConst()
{
	return RCast<const ElementType*>(m_Data);
}
