#include "RendererPCH.h"

#include "RendererCore.h"

#include "ModelManager.h"


// -----------------------------
//	Extern Variable Definitions
// -----------------------------
//
ICommandManager* GCommandManager = NULL;
IContextManager* GContextManager = NULL;
IDevice* GDevice = NULL;
IGeometryBufferManager* GGeometryManager = NULL;
IConstantBufferManager* GConstantBufferManager = NULL;
ITextureManager* GTextureManager = NULL;
ITexture* GDefaultTextures[DT_NumDefaultTextures];
IDescriptorHeap* GTextureHeap = NULL;
StaticGeometryManager GStaticGeometryManager;


// -----------------------------
//	Function Definitions
// -----------------------------
//
ITexture* GetDefaultTexture(EDefaultTexture TexID)
{
	HE_ASSERT(TexID < DT_NumDefaultTextures&& TexID > -1);
	return GDefaultTextures[TexID];
}