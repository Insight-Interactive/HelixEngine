// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "TDynamicArray.h"

const uint64 kDynArrayConstructElementCount = 2u;
const uint64 kDefaultResizeMultiplier = 2u;

#if HE_TARRAY_DEBUG_LEVEL	== 2
#	define HE_TDYNARRAY_THROW_ON_OUT_OF_BOUNDS	1
#	define HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT		1
#	define HE_TDYNARRAY_THROW_ON_FAILED_REALLOC 1
#	define HE_TDYNARRAY_WARN_ON_FAILED_REALLOC	1
#	define HE_TDYNARRAY_THROW_ON_FAILED_ALLOC	1
#	define HE_TDYNARRAY_WARN_ON_OUT_OF_BOUNDS	0

#elif HE_TARRAY_DEBUG_LEVEL == 1
#	define HE_TDYNARRAY_WARN_ON_OUT_OF_BOUNDS	1
#	define HE_TDYNARRAY_THROW_ON_OUT_OF_BOUNDS	0
#	define HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT		0
#	define HE_TDYNARRAY_THROW_ON_FAILED_REALLOC 0
#	define HE_TDYNARRAY_WARN_ON_FAILED_REALLOC	1

#elif HE_TARRAY_DEBUG_LEVEL == 0
#	define HE_TDYNARRY_THROW_ON_OUT_OF_BOUNDS	0
#	define HE_TDYNARRAY_WARN_ON_OUT_OF_BOUNDS	0
#	define HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT		0
#	define HE_TDYNARRAY_THROW_ON_FAILED_REALLOC 0
#	define HE_TDYNARRAY_WARN_ON_FAILED_REALLOC	0

#endif



template<typename ElementType>
INLINE TDynamicArray<ElementType>::TDynamicArray()
	: m_Data(NULL)
	, m_CarryCapacity(kDynArrayConstructElementCount * kDefaultResizeMultiplier)
	, m_LogicalSize(kDynArrayConstructElementCount)
	, m_FreeIndex(0)
{
	Reserve(m_CarryCapacity);
}

template<typename ElementType>
INLINE TDynamicArray<ElementType>::TDynamicArray(const uint64& InitialSize)
	: m_Data(NULL)
	, m_CarryCapacity(InitialSize* kDefaultResizeMultiplier)
	, m_LogicalSize(InitialSize)
	, m_FreeIndex(0)
{
	Reserve(m_CarryCapacity);
}

template<typename ElementType>
INLINE TDynamicArray<ElementType>::~TDynamicArray()
{
	ReleaseMemory();
}

template<typename ElementType>
INLINE ElementType& TDynamicArray<ElementType>::operator[](const uint64& Index)
{
	if (Index >= m_LogicalSize)
	{
#if HE_TDYNARRAY_THROW_ON_OUT_OF_BOUNDS
		throw;
#endif
#if HE_TDYNARRAY_WARN_ON_OUT_OF_BOUNDS 
		HE_LOG(Warning, TEXT("DynamicArray index {%u} out of bounds! DynamicArray max length=%u"), Index, Size());
#endif
	}
	return m_Data[Index];
}

template<typename ElementType>
INLINE ElementType* TDynamicArray<ElementType>::Data()
{
	return m_Data;
}

template<typename ElementType>
INLINE const ElementType* TDynamicArray<ElementType>::DataConst()
{
	return RCast<const ElementType*>(m_Data);
}

template<typename ElementType>
INLINE ElementType& TDynamicArray<ElementType>::PushBack(const ElementType& Element)
{
	m_Data[m_FreeIndex] = Element;
	++m_FreeIndex;

	ResizeIfNeeded();

	return m_Data[GetCurrentInUseIndex()];
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::PopBack()
{
	RemoveAt(GetCurrentInUseIndex());
}

template<typename ElementType>
template <typename ... Args>
INLINE ElementType& TDynamicArray<ElementType>::EmplaceBack(Args && ... InitArgs)
{
	::new(&m_Data[m_FreeIndex]) ElementType(InitArgs...);
	++m_FreeIndex;

	ResizeIfNeeded();

	return m_Data[GetCurrentInUseIndex()];
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::RemoveAt(const uint64& Index)
{
	if (Index > m_FreeIndex)
	{
#if HE_TDYNARRAY_THROW_ON_OUT_OF_BOUNDS
		throw;
#endif
#if HE_TDYNARRAY_WARN_ON_OUT_OF_BOUNDS 
		HE_LOG(Warning, TEXT("DynamicArray index {%u} out of bounds! DynamicArray max length=%u"), Index, Size());
#endif
	}

	// Delete the element.
	m_Data[Index].~ElementType();
#if HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT
	// Zero the memory so the delete it is more easily debuggable.
	ZeroMemory(&m_Data[Index], sizeof(ElementType));
#endif

	if (Index != m_LogicalSize - 1)
	{
		// Take the last element and copy it into the deleted element to fill the gap.
		CopyMemory(
			&m_Data[Index],
			&m_Data[GetCurrentInUseIndex()],
			sizeof(ElementType)
		);
		m_FreeIndex = GetCurrentInUseIndex();

#if HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT
		// Zero the memory so it is more easily debuggable.
		ZeroMemory(&m_Data[m_FreeIndex], sizeof(ElementType));
#endif
	}
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::Resize(const uint64& NewSize)
{
	if (NewSize >= m_CarryCapacity)
	{
		// Aquire additional memory from the heap.
		Reserve(NewSize * kDefaultResizeMultiplier);
	}
	else
	{
		uint64 NumElementsToDestroy = m_LogicalSize - NewSize;
		if (NumElementsToDestroy > 0)
		{
			for (uint64 i = m_LogicalSize; i > NewSize; --i)
			{
				m_Data[i].~ElementType();
			}

#if HE_TDYNARRAY_DEBUG_MEMORY_LAYOUT
			// Zero the memory so it is more easily debuggable.
			ZeroMemory(
				&m_Data[NewSize],
				NumElementsToDestroy * sizeof(ElementType)
			);
#endif
		}

		m_LogicalSize = NewSize;
	}
}

template <typename ElementType>
INLINE void TDynamicArray<ElementType>::Reserve(const uint64& NewCapacity)
{
	uint64 NewSizeInBytes = NewCapacity * sizeof(ElementType);
	if (m_Data == NULL)
	{
		m_Data = RCast<ElementType*>(HE_HeapAlloc(NewSizeInBytes));
		if (m_Data == NULL)
		{
#if HE_TDYNARRAY_THROW_ON_FAILED_ALLOC
			throw;
#endif
		}
	}
	else
	{
		void* TempData = HE_ReAlloc(m_Data, NewSizeInBytes);
		if (TempData == NULL)
		{
#if HE_TDYNARRAY_THROW_ON_FAILED_REALLOC
			throw;
#endif
		}
		m_Data = RCast<ElementType*>(TempData);
	}

	m_CarryCapacity = NewCapacity;
}

template<typename ElementType>
INLINE uint64 TDynamicArray<ElementType>::SizeInBytes() const
{
	return Size() * sizeof(ElementType);
}

template<typename ElementType>
INLINE uint64 TDynamicArray<ElementType>::Size() const
{
	return m_FreeIndex;
}

template<typename ElementType>
INLINE uint64 TDynamicArray<ElementType>::Capacity() const
{
	return m_CarryCapacity;
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::TrimExcess()
{
	Resize(m_LogicalSize);
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::ResizeIfNeeded()
{
	if (m_FreeIndex > m_LogicalSize)
	{
		++m_LogicalSize;
		Resize(m_LogicalSize);
	}
}

template<typename ElementType>
INLINE uint64 TDynamicArray<ElementType>::GetCurrentInUseIndex() const
{
	return m_FreeIndex - 1;
}

template<typename ElementType>
INLINE void TDynamicArray<ElementType>::ReleaseMemory()
{
	HE_HeapFree(m_Data);
}
