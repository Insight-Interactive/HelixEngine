#include "EnginePCH.h"
#if R_WITH_D3D12

#include "Engine/Renderer/TextureManager.h"

#include "Utility/DDSTextureLoader.h"


void FTextureManager::Initialize()
{
    uint32 MagentaPixel = 0xFFFF00FF;
    GDefaultTextures[DT_Magenta2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &MagentaPixel);
    uint32 BlackOpaqueTexel = 0xFF000000;
    GDefaultTextures[DT_BlackOpaque2D].Create2D(4, 1, 1, F_R8G8B8A8_UNorm, &BlackOpaqueTexel);
    uint32 BlackTransparentTexel = 0x00000000;
    GDefaultTextures[DT_BlackTransparent2D].Create2D( 4, 1, 1, F_R8G8B8A8_UNorm, &BlackTransparentTexel );
    uint32 WhiteOpaqueTexel = 0xFFFFFFFF;
    GDefaultTextures[DT_WhiteOpaque2D].Create2D( 4, 1, 1, F_R8G8B8A8_UNorm, &WhiteOpaqueTexel );
    uint32 WhiteTransparentTexel = 0x00FFFFFF;
    GDefaultTextures[DT_WhiteTransparent2D].Create2D( 4, 1, 1, F_R8G8B8A8_UNorm, &WhiteTransparentTexel );
    uint32 FlatNormalTexel = 0x00FF8080;
    GDefaultTextures[DT_DefaultNormalMap].Create2D( 4, 1, 1, F_R8G8B8A8_UNorm, &FlatNormalTexel );
    uint32 BlackCubeTexels[6] = {};
    GDefaultTextures[DT_BlackCubeMap].CreateCube(4, 1, 1, F_R8G8B8A8_UNorm, BlackCubeTexels);
}

void FTextureManager::UnInitialize()
{
    for (auto Iter = m_TextureCache.begin(); Iter != m_TextureCache.end(); ++Iter)
    {
        Iter->second.reset();
    }
    m_TextureCache.clear();
}

HTexture FTextureManager::LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB)
{
    return FindOrLoadTexture(FileName, Fallback, forceSRGB);
}

ManagedTexture* FTextureManager::FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB)
{
    ManagedTexture* pTexture = NULL;

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
            pTexture = new FTexture();
            pTexture->SetName( key );
            m_TextureCache[key].reset(pTexture);
        }
    }

    // TODO Filesystem readfilesync
    DataBlob Data = FileSystem::ReadRawData(FileName.c_str());
    GTextureManager.CreateTextureFromMemory( &pTexture, Data, Fallback, forceSRGB );

    // This was the first time it was requested, so indicate that the caller must read the file.
    return pTexture;
}

void FTextureManager::DestroyTexture(const String& Key)
{
    m_Mutex.Enter();

    auto Iter = m_TextureCache.find(Key);
    if (Iter != m_TextureCache.end())
        m_TextureCache.erase(Iter);

    m_Mutex.Exit();
}

void FTextureManager::CreateTextureFromMemory( ManagedTexture** pOutTexture, DataBlob& Memory, EDefaultTexture DefaultTexture, bool ForceSRGB )
{
    ID3D12Device* pD3D12Device = RCast<ID3D12Device*>( GGraphicsDevice.GetNativeDevice() );
    FTexture& pD3D12FallbackTexture = GetDefaultTexture( DefaultTexture );
    FTexture& TextureAsset = (*pOutTexture)->GetAsset();

    D3D12_CPU_DESCRIPTOR_HANDLE FallbackSRVHandle = pD3D12FallbackTexture.GetSRV();

    if (!Memory.IsValid())
    {
        TextureAsset.m_hCpuDescriptorHandle = FallbackSRVHandle;
    }
    else
    {
        // We probably have a texture to load, so let's allocate a new descriptor
        TextureAsset.m_hCpuDescriptorHandle = AllocateDescriptor( pD3D12Device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

        HRESULT hr = CreateDDSTextureFromMemory( pD3D12Device, (const uint8_t*)Memory.GetBufferPointer(), Memory.GetDataSize(),
            0, ForceSRGB, (ID3D12Resource**)(*pOutTexture)->GetAsset().GetAddressOf(), TextureAsset.m_hCpuDescriptorHandle );
        if (SUCCEEDED( hr ))
        {
            D3D12_RESOURCE_DESC Desc = ((ID3D12Resource*)TextureAsset.GetResource())->GetDesc();
            TextureAsset.m_Width = (uint32_t)Desc.Width;
            TextureAsset.m_Height = Desc.Height;
            TextureAsset.m_Depth = Desc.DepthOrArraySize;

            TextureAsset.m_IsLoading = false;
        }
        else
        {
            R_LOG( Warning, TEXT( "Failed to create dds texture from memory. Falling back to default texture: %i" ), DefaultTexture )
                pD3D12Device->CopyDescriptorsSimple( 1, TextureAsset.m_hCpuDescriptorHandle, FallbackSRVHandle,
                    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
        }
    }
    TextureAsset.AssociateWithShaderVisibleHeap();
    TextureAsset.m_IsLoading = false;
}

#endif // R_WITH_D3D12
