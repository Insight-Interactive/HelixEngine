#include "RendererPCH.h"

#include "RendererCore.h"

#include "ModelManager.h"
#include "RenderDevice.h"
#include "BatchRenderer.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "TextureManager.h"
#include "GeometryBufferManager.h"


// -----------------------------
//	Extern Variable Definitions
// -----------------------------
//
FCommandManager GCommandManager;
FContextManager GContextManager;
FRenderDevice GGraphicsDevice;
FGeometryBufferManager GGeometryManager;
FTextureManager GTextureManager;
FTexture GDefaultTextures[DT_NumDefaultTextures];
FDescriptorHeap GTextureHeap;
FStaticGeometryManager GStaticGeometryManager;
const FResolution GCommonResolutions[9] =
{
    { 800,  600  },
    { 1200, 900  },
    { 1280, 720  },
    { 1600, 900  },
    { 1920, 1080 },
    { 1920, 1200 },
    { 2560, 1440 },
    { 3440, 1440 },
    { 3840, 2160 },
};


// -----------------------------
//	Function Definitions
// -----------------------------
//
FTexture& GetDefaultTexture(EDefaultTexture TexID)
{
	HE_ASSERT(TexID < DT_NumDefaultTextures && TexID > -1);
	return GDefaultTextures[TexID];
}

#if R_WITH_D3D12
FDescriptorAllocator GDescriptorAllocator[RHT_HeapType_Count]
{
	RHT_CBV_SRV_UAV,
	RHT_Sampler,
	RHT_RTV,
	RHT_DSV,
};
#endif // R_WITH_D3D12
