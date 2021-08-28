#include "RendererPCH.h"

#include "VertexBufferD3D12.h"

#include "RendererCore.h"
#include "IDevice.h"
#include "ICommandContext.h"
#include "../D3DCommon/D3DCommon.h"


void VertexBufferD3D12::Create(const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies)
{
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
	HE_ASSERT(pID3D12Device != NULL);
	auto ResDesc = CD3DX12_RESOURCE_DESC::Buffer(VertexDataSize);
	auto HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// Initialize GPU Resource.
	//
	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pID3D12Resource)
	);
	ThrowIfFailedMsg(hr, TEXT("Failed to create default heap for vertex buffer!"));
#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(Name);
#endif // IE_DEBUG
	HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// Initialize Upload Heap.
	//
	GpuResourceD3D12 UploadHeap;
	hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(UploadHeap.GetAddressOf())
	);
	ThrowIfFailedMsg(hr, TEXT("Failed to create upload heap for vertex buffer!"));

	SubResourceData vertexData = {};
	vertexData.pData = pVerticies;
	vertexData.RowPitch = VertexDataSize;
	vertexData.SlicePitch = VertexDataSize;

	ICommandContext& InitContext = ICommandContext::Begin(L"");
	{
		InitContext.UpdateSubresources(*this, UploadHeap, 0, 0, 1, vertexData);
	}
	InitContext.End(true);
	//UploadHeap.DestroyCOMResource();

	// Initialize the Vertex Buffer View.
	//
	m_D3D12VertexBufferView.BufferLocation = m_pID3D12Resource->GetGPUVirtualAddress();
	m_D3D12VertexBufferView.StrideInBytes = VertexStrideSize;
	m_D3D12VertexBufferView.SizeInBytes = VertexDataSize;
}
