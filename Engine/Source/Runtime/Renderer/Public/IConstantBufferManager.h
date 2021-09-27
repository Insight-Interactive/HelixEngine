#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

#include "CriticalSection.h"


class RENDER_API IConstantBuffer
{
	friend class IConstantBufferManager;
public:
	template <typename BufferCastType>
	FORCEINLINE BufferCastType* GetBufferPointer();
	FORCEINLINE ConstantBufferUID GetUID() const;
	FORCEINLINE uint32 GetBufferSize() const;

protected:
	IConstantBuffer()
		: m_BufferSize(0)
		, m_UID(HE_INVALID_CONSTANT_BUFFER_HANDLE)
	{
		ZeroMemory(m_Data, sizeof(uint8) * HE_MAX_CONSTANT_BUFFER_SIZE);
	}
	~IConstantBuffer() = default;

	FORCEINLINE void SetBufferSize(uint32 BufferSize);
	FORCEINLINE void SetUID(const ConstantBufferUID& UID);

	virtual void Create(const WChar* Name, uint32 BufferSize) = 0;

	uint32 m_BufferSize;
	ConstantBufferUID m_UID;
	uint8 m_Data[HE_MAX_CONSTANT_BUFFER_SIZE];
};

class RENDER_API IConstantBufferManager
{
	friend class RenderContext;
public:
	virtual void CreateConstantBuffer(const WChar* Name, IConstantBuffer** OutBuffer, uint32 BufferSizeInBytes) = 0;
	virtual void DestroyConstantBuffer(ConstantBufferUID BufferHandle) = 0;

	virtual void Initialize() = 0;

protected:
	IConstantBufferManager()
	{
	}
	virtual ~IConstantBufferManager()
	{
	}

	ConstantBufferUID AllocBufferHandle();

private:
	static CriticalSection SBufferIdGuard;
	static ConstantBufferUID SNextAvailableBufferID;

};


//
// Inline function implementations
//

// IConstantBuffer
//

FORCEINLINE ConstantBufferUID IConstantBuffer::GetUID() const
{
	return m_UID;
}

FORCEINLINE uint32 IConstantBuffer::GetBufferSize() const
{
	return m_BufferSize;
}

template <typename BufferCastType>
FORCEINLINE BufferCastType* IConstantBuffer::GetBufferPointer()
{
	return RCast<BufferCastType*>(m_Data);
}

FORCEINLINE void IConstantBuffer::SetBufferSize(uint32 BufferSize)
{
	m_BufferSize = BufferSize;
}

FORCEINLINE void IConstantBuffer::SetUID(const ConstantBufferUID& UID)
{
	m_UID = UID;
}

// IConstantBufferManager
//

FORCEINLINE ConstantBufferUID IConstantBufferManager::AllocBufferHandle()
{
	ScopedCriticalSection Guard( SBufferIdGuard );
	SNextAvailableBufferID++;
	return SNextAvailableBufferID;
}
