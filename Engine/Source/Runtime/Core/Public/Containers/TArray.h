// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
	File: TArray.h
	Source: TArray.inl

	Author: Garrett Courtney

	Description: A static array who's size cannot be changed. Elements are allocated on the stack
	and are guaranteed to be linear in memory.
*/
#pragma once

#include "DataTypes.h"

/*
	Statically allocated array who's size cannot be changed at runtime.
*/
template <typename ElementType, uint64 NumElements>
class TArray
{
public:
	INLINE TArray();
	INLINE ~TArray();

	/*
		Returns a reference to an element at a specified index. No bounds checking.
	*/
	INLINE ElementType& operator[](const uint64& Index);

	/*
		Returns the number of the elements the array can carry.
	*/
	INLINE uint64 Length();

	/*
		Returns a reference to en element at a specified index. Bounds checking is applied.
	*/
	INLINE ElementType& At(const uint64& Index);

	/*
		Returns a pointer to the first element in the array.
	*/
	INLINE ElementType* Data();

	/*
		Returns a constant pointer to the first element in the array.
	*/
	INLINE const ElementType* DataConst();

private:
	ElementType m_Data[NumElements];
};


#include "TArray.inl"
