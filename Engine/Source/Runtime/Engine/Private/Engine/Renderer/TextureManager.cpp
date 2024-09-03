#include "EnginePCH.h"

#include "Engine/Renderer/TextureManager.h"

#include "Engine/Renderer/RendererCore.h"
#include "Engine/Renderer/Texture.h"


void FTextureManager::DestroyDefaultTextures()
{
	for (uint32 i = 0; i < DT_NumDefaultTextures; ++i)
	{
		// Release the resources but dont destroy.
		GDefaultTextures[i].Destroy();
	}
}
