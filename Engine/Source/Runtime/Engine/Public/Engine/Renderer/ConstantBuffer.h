// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "GpuResource.h"

#include "System.h"
#include "RendererCore.h"
#include "RenderDevice.h"
#include "CriticalSection.h"


/*
	Base class all constant buffers should derive from.
*/
class FConstantBufferInterface : public FGpuResource
{
	friend class FMaterial;
public:
	virtual ~FConstantBufferInterface()
	{
	}

	virtual void Create( const WChar* Name, uint32 Size ) = 0;
	virtual void Destroy() = 0;
	virtual void UploadBuffer() = 0;

	/*
		Returns true if the buffer is ready to be used too, false if not.
	*/
	bool IsValid() const;
	bool GetIsDirty() const;
	uint32 GetBufferSize() const;

protected:
	FConstantBufferInterface()
		: m_BufferSize( 0 )
		, m_IsDirty( true )
#if R_WITH_D3D12
		, m_pWritePointer( nullptr )
#endif
	{
	}

#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE CBV() const;
	void* GetGPUWritePointer();
#endif
	void CreateAPIBuffer( const WChar* Name );
	static uint32 GetAlignedBufferSize( const uint32 BufferSize, const uint32 Alignment );

protected:
#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE m_CBV;
	void* m_pWritePointer;
#endif
	uint32 m_BufferSize;
	bool m_IsDirty;

};


/*
	GPU constant buffer whose data is heap allocated.
*/
class FConstantBuffer : public FConstantBufferInterface
{
public:
	FConstantBuffer()
		: m_pData( nullptr )
	{
	}
	virtual ~FConstantBuffer()
	{
	}

	virtual void Create( const WChar* Name, uint32 Size ) override;
	virtual void Destroy() override;
	virtual void UploadBuffer() override;

	uint8* operator->() { return GetBufferPointer(); }
	uint8* GetBufferPointer();

private:
	uint8* m_pData;

};


/*
	GPU constant buffer whose size is known at compile time and cannot change.
*/
template <typename BufferDataType>
class TConstantBuffer : public FConstantBufferInterface
{
	friend class TConstantBufferManager;
	friend class FCommandContext;
	friend class FRenderContextFactory;
public:
	TConstantBuffer()
	{
		ZeroMemory( m_Data, sizeof( m_Data ) );
	}
	virtual ~TConstantBuffer()
	{
	}

	virtual void Create( const WChar* Name, uint32 Size = 0/* Ignored for this buffer type.. */ ) override;
	virtual void Destroy() override;
	virtual void UploadBuffer() override;

	BufferDataType* operator->() { return GetBufferPointer(); }
	BufferDataType* GetBufferPointer();

protected:

protected:
	uint8 m_Data[sizeof( BufferDataType )];

};


//
// Inline function implementations
//


// FConstantBufferInterface
//

FORCEINLINE uint32 FConstantBufferInterface::GetBufferSize() const
{
	return m_BufferSize;
}

FORCEINLINE bool FConstantBufferInterface::IsValid() const
{
	return m_BufferSize > 0;
}

FORCEINLINE bool FConstantBufferInterface::GetIsDirty() const
{
	return m_IsDirty;
}

FORCEINLINE void FConstantBufferInterface::CreateAPIBuffer( const WChar* Name )
{
#if R_WITH_D3D12
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
	
	D3D12_RESOURCE_DESC ResDesc = { };
	ResDesc.Width				= m_BufferSize;
	ResDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;
	ResDesc.Alignment			= 0;
	ResDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	ResDesc.Height				= 1;
	ResDesc.DepthOrArraySize	= 1;
	ResDesc.MipLevels			= 1;
	ResDesc.Format				= DXGI_FORMAT_UNKNOWN;
	ResDesc.SampleDesc			= { 1, 0 };
	ResDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES HeapProps = { };
	HeapProps.Type					= D3D12_HEAP_TYPE_UPLOAD;
	HeapProps.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask		= 1;
	HeapProps.VisibleNodeMask		= 1;

	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &m_pID3D12Resource )
	);
	ThrowIfFailedMsg( hr, "Failed to create default heap for vertex buffer!" );
#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName( Name );
#endif 
	m_GpuVirtualAddress = m_pID3D12Resource->GetGPUVirtualAddress();

	D3D12_RANGE ReadRange = {};        // We do not intend to read from this resource on the CPU.
	hr = m_pID3D12Resource->Map( 0, &ReadRange, &m_pWritePointer );
	ThrowIfFailedMsg( hr, "Failed to create committed resource for constant buffer!" );

	m_CBV = AllocateDescriptor( pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.SizeInBytes		= m_BufferSize;
	CBVDesc.BufferLocation	= m_pID3D12Resource->GetGPUVirtualAddress();
	pID3D12Device->CreateConstantBufferView( &CBVDesc, m_CBV );
#endif // R_WITH_D3D12
}

#if R_WITH_D3D12
FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE FConstantBufferInterface::CBV() const
{
	return m_CBV;
}

FORCEINLINE void* FConstantBufferInterface::GetGPUWritePointer()
{
	return m_pWritePointer;
}
#endif // R_WITH_D3D12

/* static */ FORCEINLINE uint32 FConstantBufferInterface::GetAlignedBufferSize( const uint32 BufferSize, const uint32 Alignment )
{
	return (BufferSize + (Alignment - 1)) & ~(Alignment - 1);
}


// FConstantBuffer
//

FORCEINLINE void FConstantBuffer::UploadBuffer()
{
	HE_ASSERT( IsValid() && m_pData != nullptr ); // Trying to upload a buffer that has not been created! Did you forget to call FConstantBufferInterface::Create()?

	if (!GetIsDirty())
		return;

#if R_WITH_D3D12
	memcpy( GetGPUWritePointer(), (const void*)m_pData, GetBufferSize() );
#endif // R_WITH_D3D12

	m_IsDirty = false;
}

FORCEINLINE void FConstantBuffer::Create( const WChar* Name, uint32 Size )
{
	if (m_pData != nullptr)
	{
		R_LOG( Warning, TEXT( "Trying to re-create a constant buffer that has already been inititlized." ) );
		HE_ASSERT( false );
		return;
	}

	m_BufferSize = GetAlignedBufferSize( Size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT );

	// Allocate the Gpu memory.
	CreateAPIBuffer( Name );

	// Allocate the Cpu memory.
	m_pData = (uint8*)HE_HeapAlloc( Size );
	HE_ASSERT( m_pData != NULL );
	ZeroMemory( m_pData, Size );
}

FORCEINLINE void FConstantBuffer::Destroy()
{
	if (m_pData != NULL)
		HE_HeapFree( m_pData );
}

FORCEINLINE uint8* FConstantBuffer::GetBufferPointer()
{
	m_IsDirty = true;
	return m_pData;
}


// TConstantBuffer
//

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::Destroy()
{

}

template <typename BufferDataType>
FORCEINLINE BufferDataType* TConstantBuffer<BufferDataType>::GetBufferPointer()
{
	m_IsDirty = true;
	return RCast<BufferDataType*>( m_Data );
}

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::UploadBuffer()
{
	HE_ASSERT( IsValid() ); // Trying to upload a buffer that has not been created! Did you forget to call FConstantBufferInterface::Create()?

	if (!GetIsDirty())
		return;

#if R_WITH_D3D12
	memcpy( GetGPUWritePointer(), (const void*)m_Data, GetBufferSize() );
#endif // R_WITH_D3D12

	m_IsDirty = false;
}

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::Create( const WChar* Name, uint32 Size/*Ignored*/ )
{
	if (m_pWritePointer != nullptr)
	{
		R_LOG( Warning, TEXT( "Trying to re-create a constant buffer that has already been inititlized." ) );
		HE_ASSERT( false );
		return;
	}

	m_BufferSize = GetAlignedBufferSize( sizeof( BufferDataType ), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT );

	CreateAPIBuffer( Name );
}
