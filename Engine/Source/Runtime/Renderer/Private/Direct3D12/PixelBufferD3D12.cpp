#include "RendererPCH.h"

#include "PixelBufferD3D12.h"
#include "DeviceD3D12.h"


ResourceDesc PixelBufferD3D12::DescribeTex2D(uint32 Width, uint32 Height, uint32 DepthOrArraySize, uint32 NumMips, EFormat Format, uint32 Flags)
{
	m_Width = Width;
	m_Height = Height;
	m_ArraySize = DepthOrArraySize;
	m_Format = Format;

	ResourceDesc Desc = {};
	Desc.Alignment = 0;
	Desc.DepthOrArraySize = (uint16)DepthOrArraySize;
	Desc.Dimension = RD_Texture_2D;
	Desc.Flags = (EResourceFlags)Flags;
	Desc.Format = GetBaseFormat(Format);
	Desc.Height = (uint32)Height;
	Desc.Layout = TL_Unknown;
	Desc.MipLevels = (uint16)NumMips;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Width = (uint64)Width;

	return Desc;
}

void PixelBufferD3D12::AssociateWithResource(IDevice* pDevice, const WChar* Name, void* pResource, EResourceState CurrentState)
{
	HE_ASSERT(pResource != NULL);

	ID3D12Resource* pD3D12Resource = RCast<ID3D12Resource*>(pResource);
	m_pID3D12Resource.Attach(pD3D12Resource);

	D3D12_RESOURCE_DESC Desc = pD3D12Resource->GetDesc();

	m_UsageState = CurrentState;

	m_Width = (uint32)Desc.Width;
	m_Height = Desc.Height;
	m_ArraySize = Desc.DepthOrArraySize;
	m_Format = (EFormat)Desc.Format;

#if HE_DEBUG
	pD3D12Resource->SetName(Name);
#endif // IE_DEBUG
}

void PixelBufferD3D12::CreateTextureResource(IDevice* pDevice, const WChar* Name, const ResourceDesc& ResourceDesc, const ClearValue& ClearValue)
{
	HE_ASSERT(pDevice != NULL);

	DeviceD3D12* pD3D12Device = DCast<DeviceD3D12*>(pDevice);
	HE_ASSERT(pD3D12Device != NULL);

	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(pD3D12Device->GetNativeDevice());
	HE_ASSERT(pID3D12Device != NULL);

	D3D12_CLEAR_VALUE D3D12ClearVal;
	ZeroMemory(&D3D12ClearVal, sizeof(D3D12_CLEAR_VALUE));
	memcpy(D3D12ClearVal.Color, ClearValue.Color, 4 * sizeof(float));
	D3D12ClearVal.DepthStencil.Depth = ClearValue.DepthStencil.Depth;
	D3D12ClearVal.DepthStencil.Stencil = ClearValue.DepthStencil.Stencil;
	D3D12ClearVal.Format = (DXGI_FORMAT)ClearValue.Format;

	{
		CD3DX12_HEAP_PROPERTIES HeapProps(D3D12_HEAP_TYPE_DEFAULT);
		HRESULT hr = pID3D12Device->CreateCommittedResource(
			&HeapProps,
			D3D12_HEAP_FLAG_NONE,
			RCast<const D3D12_RESOURCE_DESC*>(&ResourceDesc),
			(D3D12_RESOURCE_STATES)RS_Common,
			&D3D12ClearVal,
			IID_PPV_ARGS(&m_pID3D12Resource)
		);
		ThrowIfFailedMsg(hr, TEXT("Failed to create committed GPU resource!"));
	}
	
	m_UsageState = RS_Common;
	m_GpuVirtualAddress = HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL;

#if R_DEBUG_GPU_RESOURCES
	m_pID3D12Resource->SetName(Name);
#endif
}

