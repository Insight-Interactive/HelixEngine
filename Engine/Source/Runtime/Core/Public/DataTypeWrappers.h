// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Ref.h"
#include "Containers/TDynamicArray.h"


typedef Ref< TDynamicArray<uint8> > ByteArray;


/*
	A byte array who's memory is reference counted.
*/
struct DataBlob
{
	friend class FileSystem;
	const uint32 kInvalidDataSize = -1;
public:
	DataBlob()
		: m_DataSize(0)
	{
	}
	~DataBlob() = default;

	inline bool IsValid()
	{
		return m_ByteArray != NULL && m_DataSize != kInvalidDataSize;
	}
	inline void Invalidate()
	{
		m_ByteArray.Reset();
		m_ByteArray = NULL;
		m_DataSize = kInvalidDataSize;
	}
	inline TDynamicArray<uint8>* operator->()
	{
		return m_ByteArray.Get();
	}
	inline uint8* GetBufferPointer()
	{
		return m_ByteArray.Get()->Data();
	}
	inline uint32 GetDataSize() const
	{
		return m_DataSize;
	}

private:
	ByteArray m_ByteArray;
	uint32 m_DataSize;
	
};
