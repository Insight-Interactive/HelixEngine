#include "RendererPCH.h"
#if R_WITH_D3D12

#include "VertexBuffer.h"

#include "RendererCore.h"
#include "RenderDevice.h"
#include "CommandContext.h"


// FVertexBuffer
//

void FVertexBuffer::Create( const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies )
{
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());
	HE_ASSERT(pID3D12Device != NULL);
	auto ResDesc	= CD3DX12_RESOURCE_DESC::Buffer(VertexDataSize);
	auto HeapProps	= CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


	// Initialize Gpu resource.
	//
	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pID3D12Resource)
	);
	ThrowIfFailedMsg(hr, "Failed to create default heap for vertex buffer!");
#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(Name);
#endif // IE_DEBUG
	HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// Initialize upload heap.
	//
	FGpuResource UploadHeap;
	hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS((ID3D12Resource**)UploadHeap.GetAddressOf())
	);
	ThrowIfFailedMsg(hr, "Failed to create upload heap for vertex buffer!");

	FSubResourceData VertexData = {};
	VertexData.pData		= pVerticies;
	VertexData.RowPitch		= VertexDataSize;
	VertexData.SlicePitch	= VertexDataSize;

	FCommandContext& InitContext = FCommandContext::Begin(L"Update Vertex Buffer Subresource");
	{
		InitContext.UpdateSubresources(*this, UploadHeap, 0, 0, 1, VertexData);
	}
	InitContext.End(true);

	// Initialize the vertex buffer view.
	m_D3D12VertexBufferView.BufferLocation	= m_pID3D12Resource->GetGPUVirtualAddress();
	m_D3D12VertexBufferView.StrideInBytes	= VertexStrideSize;
	m_D3D12VertexBufferView.SizeInBytes		= VertexDataSize;
}


// FDynamicVertexBuffer
//

void FDynamicVertexBuffer::Create( const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies ) 
{
	m_pVertexData = (uint8*)pVerticies;
	m_VertexDataSize = VertexDataSize;

	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
	HE_ASSERT( pID3D12Device != NULL );
	auto ResDesc = CD3DX12_RESOURCE_DESC::Buffer( VertexDataSize );
	auto HeapProps = CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD );

	// Create the upload heap.
	//
	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &m_pID3D12Resource )
	);
	ThrowIfFailedMsg( hr, "Failed to create default heap for dynamic vertex buffer!" );
#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName( Name );
#endif 
	m_GpuVirtualAddress = m_pID3D12Resource->GetGPUVirtualAddress();

	D3D12_RANGE ReadRange = {};        // We do not intend to read from this resource on the CPU.
	hr = m_pID3D12Resource->Map( 0, &ReadRange, (void**)&m_WritePtr );
	ThrowIfFailedMsg( hr, "Failed to create committed resource for dynamic vertex buffer buffer!" );

	// Initialize the vertex buffer view.
	m_D3D12VertexBufferView.BufferLocation = m_pID3D12Resource->GetGPUVirtualAddress();
	m_D3D12VertexBufferView.StrideInBytes = VertexStrideSize;
	m_D3D12VertexBufferView.SizeInBytes = VertexDataSize;
}

#endif // R_WITH_D3D12
