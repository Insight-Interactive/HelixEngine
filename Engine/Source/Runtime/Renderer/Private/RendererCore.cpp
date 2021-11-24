#include "RendererPCH.h"

#include "RendererCore.h"

#include "ModelManager.h"
#include "RenderDevice.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "GeometryBufferManager.h"
#include "TextureManager.h"


// -----------------------------
//	Extern Variable Definitions
// -----------------------------
//
FCommandManager GCommandManager;
FContextManager GContextManager;
FRenderDevice GGraphicsDevice;
FGeometryBufferManager GGeometryManager;
FTextureManager GTextureManager;
HTexture GDefaultTextures[DT_NumDefaultTextures];
FDescriptorHeap GTextureHeap;
FStaticGeometryManager GStaticGeometryManager;


// -----------------------------
//	Function Definitions
// -----------------------------
//
HTexture& GetDefaultTexture(EDefaultTexture TexID)
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
