#include "RendererPCH.h"

#include "ConstantBufferManagerD3D12.h"

#include "BackendCoreD3D12.h"

#include "RendererCore.h"
#include "IDevice.h"

// -------------------
// ConstantBufferD3D12
// -------------------

void ConstantBufferD3D12::Create(const WChar* Name, uint32 BufferSize)
{
	SetBufferSize(GetAlignedBufferSize(BufferSize));

	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
	auto ResDesc = CD3DX12_RESOURCE_DESC::Buffer(GetBufferSize());
	auto HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	HRESULT hr = pID3D12Device->CreateCommittedResource(
		&HeapProps,
		D3D12_HEAP_FLAG_NONE,
		&ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pID3D12Resource)
	);
	ThrowIfFailedMsg(hr, TEXT("Failed to create default heap for vertex buffer!"));

#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(Name);
#endif 

	m_GpuVirtualAddress = m_pID3D12Resource->GetGPUVirtualAddress();

	CD3DX12_RANGE ReadRange(0, 0);        // We do not intend to read from this resource on the CPU.
	hr = m_pID3D12Resource->Map(0, &ReadRange, &m_pWritePointer);
	ThrowIfFailedMsg(hr, TEXT("Failed to create committed resource for constant buffer."));

	m_CBV = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.SizeInBytes = GetBufferSize();
	CBVDesc.BufferLocation = m_pID3D12Resource->GetGPUVirtualAddress();
	pID3D12Device->CreateConstantBufferView(&CBVDesc, m_CBV);
}


// ---------------------------
// ConstantBufferManagerD3D12
// ---------------------------

void ConstantBufferManagerD3D12::Initialize()
{
	m_pID3D12DeviceRef = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
	HE_ASSERT(m_pID3D12DeviceRef != NULL);
}

void ConstantBufferManagerD3D12::CreateConstantBuffer(const WChar* Name, IConstantBuffer** OutBuffer, uint32 BufferSizeInBytes)
{
	ConstantBufferUID NewID = s_NextAvailableBufferID++;

	auto InsertResult = m_ConstantBufferLUT.try_emplace(NewID, ConstantBufferD3D12{});
	HE_ASSERT(InsertResult.second == true);

	ConstantBufferD3D12& ConstBuffer = m_ConstantBufferLUT.at(NewID);
	ConstBuffer.SetUID(NewID);
	ConstBuffer.Create(Name, BufferSizeInBytes);

	(*OutBuffer) = &ConstBuffer;
}


void ConstantBufferManagerD3D12::DestroyConstantBuffer(ConstantBufferUID BufferHandle)
{
	HE_ASSERT(BufferHandle != HE_INVALID_CONSTANT_BUFFER_HANDLE); // Trying to destroy a constant buffer with an invalid handle.

	auto Iter = m_ConstantBufferLUT.find(BufferHandle);
	if (Iter != m_ConstantBufferLUT.end())
	{
		m_ConstantBufferLUT.erase(Iter);
	}
}