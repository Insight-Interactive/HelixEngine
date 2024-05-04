// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


template <size_t S>
struct IE_ENUM_FLAG_INTEGER_FOR_SIZE;

template <>
struct IE_ENUM_FLAG_INTEGER_FOR_SIZE<1>
{
	typedef int8 type;
};

template <>
struct IE_ENUM_FLAG_INTEGER_FOR_SIZE<2>
{
	typedef int16 type;
};

template <>
struct IE_ENUM_FLAG_INTEGER_FOR_SIZE<4>
{
	typedef int32 type;
};

template <>
struct IE_ENUM_FLAG_INTEGER_FOR_SIZE<8>
{
	typedef int64 type;
};
// used as an approximation of std::underlying_type<T>
template <class T>
struct IE_ENUM_FLAG_SIZED_INTEGER
{
	typedef typename IE_ENUM_FLAG_INTEGER_FOR_SIZE<sizeof(T)>::type type;
};
#define HE_DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
extern "C++" { \
	inline constexpr ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) | ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
	inline ENUMTYPE& operator |= (ENUMTYPE& a, ENUMTYPE b) throw() { return (ENUMTYPE&)(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type&)a) |= ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
	inline constexpr ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) & ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
	inline ENUMTYPE& operator &= (ENUMTYPE& a, ENUMTYPE b) throw() { return (ENUMTYPE&)(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type&)a) &= ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
	inline constexpr ENUMTYPE operator ~ (ENUMTYPE a) throw() { return ENUMTYPE(~((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)); } \
	inline constexpr ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) throw() { return ENUMTYPE(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a) ^ ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
	inline ENUMTYPE& operator ^= (ENUMTYPE& a, ENUMTYPE b) throw() { return (ENUMTYPE&)(((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type&)a) ^= ((IE_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); } \
}
