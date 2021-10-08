#include "RendererPCH.h"

#include "RendererCore.h"

#include "ModelManager.h"


// -----------------------------
//	Extern Variable Definitions
// -----------------------------
//
FCommandManager* GCommandManager = NULL;
FContextManager* GContextManager = NULL;
FRenderDevice* GDevice = NULL;
FGeometryBufferManager* GGeometryManager = NULL;
FConstantBufferManager* GConstantBufferManager = NULL;
FTextureManager* GTextureManager = NULL;
HTexture* GDefaultTextures[DT_NumDefaultTextures];
FDescriptorHeap* GTextureHeap = NULL;
FStaticGeometryManager GStaticGeometryManager;


// -----------------------------
//	Function Definitions
// -----------------------------
//
HTexture* GetDefaultTexture(EDefaultTexture TexID)
{
	HE_ASSERT(TexID < DT_NumDefaultTextures&& TexID > -1);
	return GDefaultTextures[TexID];
}