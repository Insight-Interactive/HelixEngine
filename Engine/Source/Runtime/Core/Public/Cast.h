// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	File: Cast.h
	Source: None

	Author: Garrett Courtney

	Description: Wrapper around standard C++ type casts for more readable casts.
*/
#pragma once
#include "DataTypes.h"

/*
	Performs a const_cast on specified value.
*/
template <typename As, typename Value>
FORCEINLINE CEXPR As CCast(Value* Val)
{
	return const_cast<As>(Val);
}

/*
	Performs a reinterpret_cast on specified value.
*/
template <typename As, typename Value>
FORCEINLINE CEXPR As RCast(Value* Val)
{
	return reinterpret_cast<As>(Val);
}

/*
	Performs a dynamic_cast on specified value.
*/
template <typename As, typename Value>
FORCEINLINE CEXPR As DCast(Value* Val)
{
	return dynamic_cast<As>(Val);
}

/*
	Performs a static_cast on specified value.
*/
template <typename As, typename Value>
FORCEINLINE CEXPR As SCast(Value& Val)
{
	return static_cast<As>(Val);
}
