#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "GpuResource.h"

#include "RendererCore.h"
#include "CriticalSection.h"
#include "CommandManager.h"
#include "RenderDevice.h"


class FConstantBufferInterface : public FGpuResource
{
public:
	virtual void UploadBuffer() = 0;

protected:
	FConstantBufferInterface()
	{
	}
	virtual ~FConstantBufferInterface()
	{
	}
};


template <typename BufferDataType>
class RENDER_API TConstantBuffer : public FConstantBufferInterface
{
	friend class TConstantBufferManager;
	friend class FCommandContext;
	friend class FRenderContextFactory;
public:
	TConstantBuffer()
		: m_BufferSize( 0 )
		, m_UID( HE_INVALID_CONSTANT_BUFFER_HANDLE )
#if R_WITH_D3D12
		, m_pWritePointer( NULL )
#endif
	{
		ZeroMemory( m_Data, sizeof( m_Data ) );
	}
	virtual ~TConstantBuffer()
	{
	}

	void Create( const WChar* Name );
	void Destroy();

	BufferDataType* GetBufferPointer();
	ConstantBufferUID GetUID() const;
	uint32 GetBufferSize() const;

#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE CBV() const;
	void* GetGPUWritePointer();
#endif

protected:
	void SetBufferSize( uint32 BufferSize );
	void SetUID( const ConstantBufferUID& UID );

	virtual void UploadBuffer() override;

	uint32 m_BufferSize;
	ConstantBufferUID m_UID;
	uint8 m_Data[sizeof(BufferDataType)];

protected:
#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE m_CBV;
	void* m_pWritePointer;
#endif
private:
	static ConstantBufferUID AllocBufferUID();

	static CriticalSection SBufferIdGuard;
	static ConstantBufferUID SNextAvailableBufferID;

};

template <typename BufferDataType>
CriticalSection TConstantBuffer<BufferDataType>::SBufferIdGuard;
template <typename BufferDataType>
ConstantBufferUID TConstantBuffer<BufferDataType>::SNextAvailableBufferID = 0;


//
// Inline function implementations
//

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::Destroy()
{

}

template <typename BufferDataType>
FORCEINLINE ConstantBufferUID TConstantBuffer<BufferDataType>::GetUID() const
{
	return m_UID;
}

template <typename BufferDataType>
FORCEINLINE uint32 TConstantBuffer<BufferDataType>::GetBufferSize() const
{
	return m_BufferSize;
}

template <typename BufferDataType>
FORCEINLINE BufferDataType* TConstantBuffer<BufferDataType>::GetBufferPointer()
{
	return RCast<BufferDataType*>( m_Data );
}

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::SetBufferSize( uint32 BufferSize )
{
	m_BufferSize = BufferSize;
}

template <typename BufferDataType>
FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE TConstantBuffer<BufferDataType>::CBV() const
{
	return m_CBV;
}

#if R_WITH_D3D12
template <typename BufferDataType>
FORCEINLINE void* TConstantBuffer<BufferDataType>::GetGPUWritePointer()
{
	return m_pWritePointer;
}
#endif

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::UploadBuffer()
{
#if R_WITH_D3D12
	memcpy( GetGPUWritePointer(), (const void*)m_Data, GetBufferSize() );
#endif
}

template <typename BufferDataType>
FORCEINLINE void TConstantBuffer<BufferDataType>::SetUID( const ConstantBufferUID& UID )
{
	m_UID = UID;
}

template <typename BufferDataType>
/*static*/ FORCEINLINE ConstantBufferUID TConstantBuffer<BufferDataType>::AllocBufferUID()
{
	ScopedCriticalSection Guard( SBufferIdGuard );
	SNextAvailableBufferID++;
	return SNextAvailableBufferID;
}


//
// Helpers

constexpr FORCEINLINE uint32 GetAlignedBufferSize( const uint32 BufferSize, const uint32 Alignment )
{
	return (BufferSize + (Alignment - 1)) & ~(Alignment - 1);
}


//
// API

#if R_WITH_D3D12

template <typename BufferDataType>
void TConstantBuffer<BufferDataType>::Create( const WChar* Name )
{
	if (GetUID() != HE_INVALID_CONSTANT_BUFFER_HANDLE)
	{
		R_LOG( Warning, TEXT( "Trying to re-create a constant buffer that has already been inititlized." ) );
		return;
	}
	SetUID( AllocBufferUID() );

	SetBufferSize( GetAlignedBufferSize( sizeof( BufferDataType ), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT ) );

	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
	D3D12_RESOURCE_DESC ResDesc = { 0 };
	ResDesc.Width = GetBufferSize();
	ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	ResDesc.Alignment = 0;
	ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResDesc.Height = 1;
	ResDesc.DepthOrArraySize = 1;
	ResDesc.MipLevels = 1;
	ResDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResDesc.SampleDesc = { 1, 0 };
	ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES HeapProps = { 0 };
	HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;

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

	D3D12_RANGE ReadRange = { 0 };        // We do not intend to read from this resource on the CPU.
	hr = m_pID3D12Resource->Map( 0, &ReadRange, &m_pWritePointer );
	ThrowIfFailedMsg( hr, "Failed to create committed resource for constant buffer." );

	m_CBV = AllocateDescriptor( pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.SizeInBytes = GetBufferSize();
	CBVDesc.BufferLocation = m_pID3D12Resource->GetGPUVirtualAddress();
	pID3D12Device->CreateConstantBufferView( &CBVDesc, m_CBV );
}

#endif // R_WITH_D3D12
