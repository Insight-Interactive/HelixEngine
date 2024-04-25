#include "RendererPCH.h"

#include "TextureManager.h"

#include "RendererCore.h"
#include "Texture.h"


void FTextureManager::DestroyDefaultTextures()
{
	for (uint32 i = 0; i < DT_NumDefaultTextures; ++i)
	{
		// Release the resources but dont destroy.
		GDefaultTextures[i].Destroy();
	}
}
