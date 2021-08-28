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
	{
		m_ByteArray = std::make_shared<TDynamicArray<uint8>>();
	}
	~DataBlob() = default;

	inline bool IsValid()
	{
		return m_ByteArray != NULL && m_DataSize != kInvalidDataSize;
	}
	inline void Invalidate()
	{
		m_ByteArray.reset();
		m_ByteArray = NULL;
		m_DataSize = kInvalidDataSize;
	}
	inline TDynamicArray<uint8>* operator->()
	{
		return m_ByteArray.get();
	}
	inline uint8* GetBufferPointer()
	{
		return m_ByteArray.get()->Data();
	}
	inline size_t GetDataSize() const
	{
		return m_DataSize;
	}

private:
	ByteArray m_ByteArray;
	uint32 m_DataSize;
	
};
