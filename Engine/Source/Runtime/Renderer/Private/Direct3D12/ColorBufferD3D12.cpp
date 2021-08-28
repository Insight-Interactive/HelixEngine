#include "RendererPCH.h"

#include "ColorBufferD3D12.h"
#include "IDevice.h"
#include "RendererCore.h"
#include "BackendCoreD3D12.h"

ColorBufferD3D12::ColorBufferD3D12()
{
    m_RTVHandle.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    m_SRVHandle.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    for (int i = 0; i < _countof(m_UAVHandle); ++i)
        m_UAVHandle[i].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
}

ColorBufferD3D12::~ColorBufferD3D12()
{
}

void ColorBufferD3D12::CreateFromSwapChain(IDevice* pDevice, const TChar* Name, void* pResource)
{
    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(pDevice->GetNativeDevice());
    AssociateWithResource(pDevice, Name, pResource, RS_Present);

    m_RTVHandle = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    pID3D12Device->CreateRenderTargetView(RCast<ID3D12Resource*>(pResource), NULL, m_RTVHandle);
}

void ColorBufferD3D12::Create(IDevice* pDevice, const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format)
{
    NumMips = (NumMips == 0 ? ComputeNumMips(Width, Height) : NumMips);
    EResourceFlags Flags = CombineResourceFlags();
    ResourceDesc ResourceDesc = DescribeTex2D(Width, Height, 1, NumMips, Format, Flags);

    ResourceDesc.SampleDesc.Count = m_FragmentCount;
    ResourceDesc.SampleDesc.Quality = 0;

    ClearValue ClearValue = {};
    ClearValue.Format = Format;
    ClearValue.Color[0] = m_ClearColor.R;
    ClearValue.Color[1] = m_ClearColor.G;
    ClearValue.Color[2] = m_ClearColor.B;
    ClearValue.Color[3] = m_ClearColor.A;


    CreateTextureResource(GDevice, Name, ResourceDesc, ClearValue);
    CreateDerivedViews(GDevice, Format, 1, NumMips);
}

void ColorBufferD3D12::DestroyAPIResource()
{
    HE_COM_SAFE_RELEASE( m_pID3D12Resource );
}

void ColorBufferD3D12::CreateDerivedViews(IDevice* pDevice, EFormat Format, uint32 ArraySize, uint32 NumMips)
{
    HE_ASSERT(ArraySize == 1 || NumMips == 1); // We don't support auto-mips on texture arrays.
    HE_ASSERT(pDevice != NULL);

    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(pDevice->GetNativeDevice());

    m_NumMipMaps = NumMips - 1;


    D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

    RTVDesc.Format = (DXGI_FORMAT)Format;
    UAVDesc.Format = (DXGI_FORMAT)GetUAVFormat(Format);
    SRVDesc.Format = (DXGI_FORMAT)Format;
    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    if (ArraySize > 1)
    {
        RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
        RTVDesc.Texture2DArray.MipSlice = 0;
        RTVDesc.Texture2DArray.FirstArraySlice = 0;
        RTVDesc.Texture2DArray.ArraySize = (UINT)ArraySize;

        UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
        UAVDesc.Texture2DArray.MipSlice = 0;
        UAVDesc.Texture2DArray.FirstArraySlice = 0;
        UAVDesc.Texture2DArray.ArraySize = (UINT)ArraySize;

        SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
        SRVDesc.Texture2DArray.MipLevels = NumMips;
        SRVDesc.Texture2DArray.MostDetailedMip = 0;
        SRVDesc.Texture2DArray.FirstArraySlice = 0;
        SRVDesc.Texture2DArray.ArraySize = (UINT)ArraySize;
    }
    else if (m_FragmentCount > 1)
    {
        RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
        SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
    }
    else
    {
        RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        RTVDesc.Texture2D.MipSlice = 0;

        UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
        UAVDesc.Texture2D.MipSlice = 0;

        SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = NumMips;
        SRVDesc.Texture2D.MostDetailedMip = 0;
    }

    if (m_SRVHandle.ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
    {
        m_RTVHandle = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_SRVHandle = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    ID3D12Resource* Resource = m_pID3D12Resource.Get();

    // Create the render target view
    pID3D12Device->CreateRenderTargetView(Resource, &RTVDesc, m_RTVHandle);

    // Create the shader resource view
    pID3D12Device->CreateShaderResourceView(Resource, &SRVDesc, m_SRVHandle);

    if (m_FragmentCount > 1)
        return;

    // Create the UAVs for each mip level (RWTexture2D)
    for (uint32_t i = 0; i < NumMips; ++i)
    {
        if (m_UAVHandle[i].ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
            m_UAVHandle[i] = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        pID3D12Device->CreateUnorderedAccessView(Resource, nullptr, &UAVDesc, m_UAVHandle[i]);

        UAVDesc.Texture2D.MipSlice++;
    }
}
