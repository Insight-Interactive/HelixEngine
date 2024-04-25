#include "RendererPCH.h"
#if R_WITH_D3D12

#include "Texture.h"

#include "RendererCore.h"
#include "CommandContext.h"
#include "LinearAllocator.h"
#include "RendererCore.h"
#include "RenderDevice.h"


void FTexture::Create2D(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitData)
{
	Destroy();

	ID3D12Device* pD3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());

	m_UsageState = RS_CopyDestination;

	m_Width = (uint32)Width;
	m_Height = (uint32)Height;
	m_Depth = 1;

	D3D12_RESOURCE_DESC texDesc = {};
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Width = Width;
	texDesc.Height = (UINT)Height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = (DXGI_FORMAT)Format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES HeapProps;
	HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;

	pD3D12Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &texDesc,
		(D3D12_RESOURCE_STATES)m_UsageState, nullptr, IID_PPV_ARGS(&m_pID3D12Resource));

#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(L"Texture");
#endif

	D3D12_SUBRESOURCE_DATA texResource;
	texResource.pData = InitData;
	texResource.RowPitch = RowPitchBytes;
	texResource.SlicePitch = RowPitchBytes * Height;

	uint32 NumSubresources = 1;
	UINT64 uploadBufferSize = GetRequiredIntermediateSize((ID3D12Resource*)GetResource(), 0, NumSubresources);

	FCommandContext& InitContext = FCommandContext::Begin(L"Texture Init");
	{
		// copy data to the intermediate upload heap and then schedule a copy from the upload heap to the default texture
		DynAlloc mem = InitContext.ReserveUploadMemory(uploadBufferSize);
		UpdateSubresources((ID3D12GraphicsCommandList*)InitContext.GetNativeContext(), (ID3D12Resource*)GetResource(), (ID3D12Resource*)mem.Buffer.GetResource(), 0, 0, NumSubresources, &texResource);
		InitContext.TransitionResource(*this, RS_GenericRead);
	}
	// Execute the command list and wait for it to finish so we can release the upload buffer
	InitContext.End(true);

	if (m_hCpuDescriptorHandle.ptr == HE_INVALID_GPU_ADDRESS)
		m_hCpuDescriptorHandle = AllocateDescriptor(pD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pD3D12Device->CreateShaderResourceView(m_pID3D12Resource.Get(), nullptr, m_hCpuDescriptorHandle);

	AssociateWithShaderVisibleHeap();
}

void FTexture::CreateCube(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitialData)
{
	Destroy();

	ID3D12Device* pD3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());

	m_UsageState = RS_CopyDestination;

	m_Width = (uint32_t)Width;
	m_Height = (uint32_t)Height;
	m_Depth = 6;

	D3D12_RESOURCE_DESC texDesc = {};
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Width = Width;
	texDesc.Height = (UINT)Height;
	texDesc.DepthOrArraySize = 6;
	texDesc.MipLevels = 1;
	texDesc.Format = (DXGI_FORMAT)Format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES HeapProps;
	HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProps.CreationNodeMask = 1;
	HeapProps.VisibleNodeMask = 1;


	HRESULT hr = pD3D12Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &texDesc,
		(D3D12_RESOURCE_STATES)m_UsageState, nullptr, IID_PPV_ARGS(&m_pID3D12Resource));
	ThrowIfFailedMsg(hr, "Failed to create committed resource.");

#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(L"Texture");
#endif

	D3D12_SUBRESOURCE_DATA texResource;
	texResource.pData = InitialData;
	texResource.RowPitch = RowPitchBytes;
	texResource.SlicePitch = texResource.RowPitch * Height;

	uint32 NumSubresources = 1;
	UINT64 uploadBufferSize = GetRequiredIntermediateSize( (ID3D12Resource*)GetResource(), 0, NumSubresources);

	FCommandContext& InitContext = FCommandContext::Begin(L"Texture Init");
	{
		// copy data to the intermediate upload heap and then schedule a copy from the upload heap to the default texture
		DynAlloc mem = InitContext.ReserveUploadMemory(uploadBufferSize);
		UpdateSubresources((ID3D12GraphicsCommandList*)InitContext.GetNativeContext(), (ID3D12Resource*)GetResource(), (ID3D12Resource*)mem.Buffer.GetResource(), 0, 0, NumSubresources, &texResource);
		InitContext.TransitionResource(*this, RS_GenericRead);
	}
	// Execute the command list and wait for it to finish so we can release the upload buffer
	InitContext.End(true);

	if (m_hCpuDescriptorHandle.ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_hCpuDescriptorHandle = AllocateDescriptor(pD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = (DXGI_FORMAT)Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	pD3D12Device->CreateShaderResourceView(m_pID3D12Resource.Get(), &srvDesc, m_hCpuDescriptorHandle);
}

void FTexture::Destroy()
{
	FGpuResource::Destroy();
	m_hCpuDescriptorHandle.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
}

void FTexture::AssociateWithShaderVisibleHeap()
{
	static const int s_NumTextures = 1;
	m_DescriptorHandle = GTextureHeap.Alloc(s_NumTextures);

	// TODO: In order to get a linear disciptor table this might have to move to the material class
	// So a descriptor range array in a single root parameter could fit all the descriptors inside 'SourceTextures' array
	// Command context would have to change to have a 'FDescriptorHandle' instead of a 'FTextureRef' though.
	uint32 DestCount = s_NumTextures;
	uint32 SourceCounts[s_NumTextures] = { 1 };
	const FTexture* SourceTextures[s_NumTextures] =
	{
		this
	};
	// Copy the texture data to the texture heap.
	GGraphicsDevice.CopyDescriptors(1, &m_DescriptorHandle, &DestCount, DestCount, SourceTextures, SourceCounts, RHT_CBV_SRV_UAV);
}

#endif // R_WITH_D3D12
