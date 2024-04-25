#include "RendererPCH.h"
#if R_WITH_D3D12

#include "DepthBuffer.h"

#include "RendererCore.h"
#include "RenderDevice.h"
#include "RendererCore.h"


void FDepthBuffer::Create(const WChar* Name, uint32 Width, uint32 Height, EFormat Format)
{
    FResourceDesc ResourceDesc = DescribeTex2D(Width, Height, 1, 1, Format, RF_AllowDepthStencil);
    ResourceDesc.SampleDesc.Count = 1;


    FClearValue ClearVal = {};
    ClearVal.Format = Format;
    ClearVal.DepthStencil.Depth = m_ClearDepth;
    ClearVal.DepthStencil.Stencil = m_ClearStencil;
    CreateTextureResource(GGraphicsDevice, Name, ResourceDesc, ClearVal, RS_DepthWrite);
    CreateDerivedViews(GGraphicsDevice, Format);
}

void FDepthBuffer::CreateDerivedViews(FRenderDevice& Device, EFormat Format)
{
    ID3D12Device* pID3D12Device = (ID3D12Device*)Device.GetNativeDevice();
    ID3D12Resource* Resource = m_pID3D12Resource.Get();

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = (DXGI_FORMAT)GetDSVFormat(Format);
    if (Resource->GetDesc().SampleDesc.Count == 1)
    {
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;
    }
    else
    {
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
    }

    if (m_hDSV[0].ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
    {
        m_hDSV[0] = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        m_hDSV[1] = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }

    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    pID3D12Device->CreateDepthStencilView(Resource, &dsvDesc, m_hDSV[0]);

    dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
    pID3D12Device->CreateDepthStencilView(Resource, &dsvDesc, m_hDSV[1]);

    DXGI_FORMAT stencilReadFormat = (DXGI_FORMAT)GetStencilFormat(Format);
    if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
    {
        if (m_hDSV[2].ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
        {
            m_hDSV[2] = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
            m_hDSV[3] = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        }

        dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
        pID3D12Device->CreateDepthStencilView(Resource, &dsvDesc, m_hDSV[2]);

        dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
        pID3D12Device->CreateDepthStencilView(Resource, &dsvDesc, m_hDSV[3]);
    }
    else
    {
        m_hDSV[2] = m_hDSV[0];
        m_hDSV[3] = m_hDSV[1];
    }

    if (m_hDepthSRV.ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
        m_hDepthSRV = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Create the shader resource view
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = (DXGI_FORMAT)GetDepthFormat(Format);
    if (dsvDesc.ViewDimension == D3D12_DSV_DIMENSION_TEXTURE2D)
    {
        SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = 1;
    }
    else
    {
        SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
    }
    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    pID3D12Device->CreateShaderResourceView(Resource, &SRVDesc, m_hDepthSRV);

    if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
    {
        if (m_hStencilSRV.ptr == HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
            m_hStencilSRV = AllocateDescriptor(pID3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        SRVDesc.Format = stencilReadFormat;
        pID3D12Device->CreateShaderResourceView(Resource, &SRVDesc, m_hStencilSRV);
    }
}

#endif // R_WITH_D3D12
