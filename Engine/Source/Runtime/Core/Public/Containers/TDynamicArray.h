// Copyright Insight Interactive. All rights reserved.
/*
	File: TDynamicArray.h
	Source: TDynamicArray.inl

	Author: Garrertt Courtney

	Description: A dynamically resizable array who's elements are allocated on the heap and 
	guaranteed to be linear in memory.
	When no more elements are available the capacity is increased by 2x the current size of the array.
*/
#pragma once

#include "DataTypes.h"

/*
	A dynamically resizable array who's size can be changed at runtime.
*/
template <typename ElementType>
class TDynamicArray
{
public:
	INLINE TDynamicArray();
	INLINE TDynamicArray(const uint64& InitialSize);
	INLINE ~TDynamicArray();

	/*
		Returns a reference to a element in the array given it's index. 
		Should not be used to initiailize and add new elements. 
		@param [in] Index - Index to fetch from the array.
	*/
	INLINE ElementType& operator[](const uint64& Index);

	/*
		Returns a pointer to the beginning of the collection in memory.
		@returns Pointer to the beginning of the data pool.
	*/
	INLINE ElementType* Data();
	INLINE const ElementType* Data() const;

	/*
		Returns a constant pointer to the beginning of the collection in memory.
		@returns Constand pointer to the beginning of the data pool.
	*/
	INLINE const ElementType* DataConst();

	/*
		Adds a element to the back of the array. Performs a deap copy, prefer EmplaceBack where possible.
		@param [in] Element - The element to push into the list.
		@param Reference to the newly created element.
	*/
	INLINE ElementType& PushBack(const ElementType& Element);

	/*
		Removes the last element from the list and calles the destructor on it.
	*/
	INLINE void PopBack();

	/*
		Constructs an element in place at the back of the array and returns a reference to it.
		@param [in] InitArgs - Optional arguments to be forwarded to the constructor of the new element.
	*/
	template <typename ... Args>
	INLINE ElementType& EmplaceBack(Args&& ... InitArgs);

	/*
		Removes an element from the array given it's index.
		@param [in] Index - The Index to remove.
	*/
	INLINE void RemoveAt(const uint64& Index);

	/*
		Resizes the array to a new specified size.
		@param [in] NewSize - The new carying capacity of the array.
	*/
	INLINE void Resize(const uint64& NewSize);

	/*
		The additional memory for the pool that will be used to sample from.
		@param [in] NewCapacity - The new number of elements that will be in the reserve pool.
	*/
	INLINE void Reserve(const uint64& NewCapacity);

	/*
		Returns the size of the array.
	*/
	INLINE uint64 Size() const;

	/*
		Returns the size of the array in bytes.
	*/
	INLINE uint64 SizeInBytes() const;

	/*
		Returns the total number of elements the array can cary before needing to be resized.s
	*/
	INLINE uint64 Capacity() const;

	/*
		Trims excess elements from the rear of the array.
	*/
	INLINE void TrimExcess();

private:
	/*
		Querys the state of the array resizing if needed.
	*/
	INLINE void ResizeIfNeeded();

	/*
		Returns the index of the currently in use index.
	*/
	INLINE uint64 GetCurrentInUseIndex() const;

	/*
		Releases the memory pool resource.
	*/
	INLINE void ReleaseMemory();

private:
	/*
		Pointer to the beginning of the memory pool.
	*/
	ElementType* m_Data;

	/*
		The number of elements in the reserve pool.
	*/
	uint64 m_CarryCapacity;

	/*
		The next free index to insert a new element into.
	*/
	uint64 m_FreeIndex;
	
	/*
		The logical array size that elements can add into. When 
		m_LogicalSize > m_CarrayCapacity the memory pool will be resized.
	*/
	uint64 m_LogicalSize;
};

#include "TDynamicArray.inl"
