#include "EnginePCH.h"
#if R_WITH_D3D12

#include "Engine/Renderer/IndexBuffer.h"


void FIndexBuffer::Create(const WChar* Name, uint32 IndexDataSize, void* pIndices)
{
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());
	HE_ASSERT( pID3D12Device != NULL );
	auto ResDesc = CD3DX12_RESOURCE_DESC::Buffer(IndexDataSize);
	auto HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// Initialize GPU Resource.
	//
	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pID3D12Resource));
	ThrowIfFailedMsg(hr, "Failed to create commited resource!");
#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(Name);
#endif // IE_DEBUG
	HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// Initialize Upload Heap.
	//
	FGpuResource UploadHeap;
	hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( (ID3D12Resource**)UploadHeap.GetAddressOf() )
	);
	ThrowIfFailedMsg(hr, "Failed to create upload heap for vertex buffer!");

	FSubResourceData vertexData = {};
	vertexData.pData = pIndices;
	vertexData.RowPitch = IndexDataSize;
	vertexData.SlicePitch = IndexDataSize;

	FCommandContext& InitContext = FCommandContext::Begin(L"");
	{
		InitContext.UpdateSubresources(*this, UploadHeap, 0, 0, 1, vertexData);
	}
	InitContext.End(true);
	//UploadHeap.DestroyCOMResource();

	// Initialize the Index Buffer View.
	//
	m_D3D12IndexBufferView.BufferLocation = m_pID3D12Resource->GetGPUVirtualAddress();
	m_D3D12IndexBufferView.Format = (DXGI_FORMAT)F_R32_UINT;
	m_D3D12IndexBufferView.SizeInBytes = IndexDataSize;
}

#endif // R_WITH_D3D12
