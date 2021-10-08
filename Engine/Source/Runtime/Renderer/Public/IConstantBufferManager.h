#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

#include "CriticalSection.h"


class RENDER_API FConstantBuffer
{
	friend class FConstantBufferManager;
public:
	template <typename BufferCastType>
	FORCEINLINE BufferCastType* GetBufferPointer();
	FORCEINLINE ConstantBufferUID GetUID() const;
	FORCEINLINE uint32 GetBufferSize() const;

protected:
	FConstantBuffer()
		: m_BufferSize(0)
		, m_UID(HE_INVALID_CONSTANT_BUFFER_HANDLE)
	{
		ZeroMemory(m_Data, sizeof(uint8) * HE_MAX_CONSTANT_BUFFER_SIZE);
	}
	~FConstantBuffer() = default;

	FORCEINLINE void SetBufferSize(uint32 BufferSize);
	FORCEINLINE void SetUID(const ConstantBufferUID& UID);

	virtual void Create(const WChar* Name, uint32 BufferSize) = 0;

	uint32 m_BufferSize;
	ConstantBufferUID m_UID;
	uint8 m_Data[HE_MAX_CONSTANT_BUFFER_SIZE];
};

class RENDER_API FConstantBufferManager
{
	friend class FRenderContext;
public:
	virtual void CreateConstantBuffer(const WChar* Name, FConstantBuffer** OutBuffer, uint32 BufferSizeInBytes) = 0;
	virtual void DestroyConstantBuffer(ConstantBufferUID BufferHandle) = 0;

	virtual void Initialize() = 0;

protected:
	FConstantBufferManager()
	{
	}
	virtual ~FConstantBufferManager()
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

// FConstantBuffer
//

FORCEINLINE ConstantBufferUID FConstantBuffer::GetUID() const
{
	return m_UID;
}

FORCEINLINE uint32 FConstantBuffer::GetBufferSize() const
{
	return m_BufferSize;
}

template <typename BufferCastType>
FORCEINLINE BufferCastType* FConstantBuffer::GetBufferPointer()
{
	return RCast<BufferCastType*>(m_Data);
}

FORCEINLINE void FConstantBuffer::SetBufferSize(uint32 BufferSize)
{
	m_BufferSize = BufferSize;
}

FORCEINLINE void FConstantBuffer::SetUID(const ConstantBufferUID& UID)
{
	m_UID = UID;
}

// FConstantBufferManager
//

FORCEINLINE ConstantBufferUID FConstantBufferManager::AllocBufferHandle()
{
	ScopedCriticalSection Guard( SBufferIdGuard );
	SNextAvailableBufferID++;
	return SNextAvailableBufferID;
}
