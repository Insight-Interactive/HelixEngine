#include "RendererPCH.h"

#include "TextureManagerD3D12.h"

#include "RendererCore.h"
#include "BackendCoreD3D12.h"
#include "FileSystem.h"
#include "IRenderDevice.h"
#include "StringHelper.h"

#include "Utility/DDSTextureLoader.h"


void FTextureManagerD3D12::Initialize()
{
    uint32 MagentaPixel = 0xFFFF00FF;
    m_DefaultTextures[DT_Magenta2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &MagentaPixel);
    uint32 BlackOpaqueTexel = 0xFF000000;
    m_DefaultTextures[DT_BlackOpaque2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &BlackOpaqueTexel);
    uint32 BlackTransparentTexel = 0x00000000;
    m_DefaultTextures[DT_BlackTransparent2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &BlackTransparentTexel);
    uint32 WhiteOpaqueTexel = 0xFFFFFFFF;
    m_DefaultTextures[DT_WhiteOpaque2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &WhiteOpaqueTexel);
    uint32 WhiteTransparentTexel = 0x00FFFFFF;
    m_DefaultTextures[DT_WhiteTransparent2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &WhiteTransparentTexel);
    uint32 FlatNormalTexel = 0x00FF8080;
    m_DefaultTextures[DT_DefaultNormalMap].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &FlatNormalTexel);
    uint32 BlackCubeTexels[6] = {};
    m_DefaultTextures[DT_BlackCubeMap].CreateCube(4, 1, 1, F_R8G8B8A8_UNorm, BlackCubeTexels);

    for (uint32 i = 0; i < DT_NumDefaultTextures; ++i)
    {
        GDefaultTextures[i] = &m_DefaultTextures[i];
    }
}

void FTextureManagerD3D12::UnInitialize()
{
    for (auto Iter = m_TextureCache.begin(); Iter != m_TextureCache.end(); ++Iter)
    {
        Iter->second.reset();
    }
    m_TextureCache.clear();
}

HTextureRef FTextureManagerD3D12::LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB)
{
    return FindOrLoadTexture(FileName, Fallback, forceSRGB);
}

HManagedTexture* FTextureManagerD3D12::FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB)
{
    HManagedTextureD3D12* pTexture = NULL;

    {
        ScopedCriticalSection Guard( m_Mutex );

        String key = StringHelper::GetFilenameFromDirectoryNoExtension(FileName);
        if (forceSRGB)
            key += "_sRGB";

        // Search for an existing managed texture
        auto iter = m_TextureCache.find(key);
        if (iter != m_TextureCache.end())
        {
            // If a texture was already created make sure it has finished loading before
            // returning a point to it.
            pTexture = iter->second.get();
            pTexture->WaitForLoad();
            return pTexture;
        }
        else
        {
            // If it's not found, create a new managed texture and start loading it.
            pTexture = new HManagedTextureD3D12(key);
            m_TextureCache[key].reset(pTexture);
        }
    }

    // TODO Filesystem readfilesync
    DataBlob Data = FileSystem::ReadRawData(FileName.c_str());
    pTexture->CreateFromMemory(Data, Fallback, forceSRGB);

    // This was the first time it was requested, so indicate that the caller must read the file.
    return pTexture;
}

void FTextureManagerD3D12::DestroyTexture(const String& Key)
{
    m_Mutex.Enter();

    auto Iter = m_TextureCache.find(Key);
    if (Iter != m_TextureCache.end())
        m_TextureCache.erase(Iter);

    m_Mutex.Exit();
}

void HManagedTextureD3D12::CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool bForceSRGB)
{
    ID3D12Device* pD3D12Device = RCast<ID3D12Device*>( GDevice->GetNativeDevice() );
    HTextureD3D12* pD3D12FallbackTexture = DCast<HTextureD3D12*>( GetDefaultTexture(fallback) );
    
    HE_ASSERT(pD3D12FallbackTexture != NULL);
    D3D12_CPU_DESCRIPTOR_HANDLE FallbackSRVHandle = pD3D12FallbackTexture->GetSRV();

    if (!memory.IsValid())
    {
        m_hCpuDescriptorHandle = FallbackSRVHandle;
    }
    else
    {
        // We probably have a texture to load, so let's allocate a new descriptor
        m_hCpuDescriptorHandle = AllocateDescriptor(pD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        HRESULT hr = CreateDDSTextureFromMemory(pD3D12Device, (const uint8_t*)memory.GetBufferPointer(), memory.GetDataSize(),
            0, bForceSRGB, GetAddressOf(), m_hCpuDescriptorHandle);
        if (SUCCEEDED(hr))
        {
            D3D12_RESOURCE_DESC Desc = GetResource()->GetDesc();
            m_Width = (uint32_t)Desc.Width;
            m_Height = Desc.Height;
            m_Depth = Desc.DepthOrArraySize;

            m_IsValid = true;
        }
        else
        {
            R_LOG(Warning, TEXT("Failed to create dds texture from memory. Falling back to default texture: %i"), fallback)
                pD3D12Device->CopyDescriptorsSimple(1, m_hCpuDescriptorHandle, FallbackSRVHandle,
                    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
    }
    AssociateWithShaderVisibleHeap();
    m_IsLoading = false;
}
