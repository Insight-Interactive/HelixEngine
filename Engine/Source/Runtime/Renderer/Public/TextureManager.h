#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CriticalSection.h"
#include "CommonEnums.h"
#include "DataTypeWrappers.h"
#include "Texture.h"



typedef ManagedAsset<FTexture> ManagedTexture;
typedef AssetRef<FTexture> HTexture;

class RENDER_API FTextureManager
{
	friend class FRenderContextFactory;
	friend class D3D12RenderContextFactory;
	friend class FRenderContext;
public:
	FTextureManager()
	{
	}
	virtual ~FTextureManager()
	{
	}

	HTexture LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB);
	void DestroyTexture(const String& Key);

private:
	ManagedTexture* FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB);

	void CreateTextureFromMemory( ManagedTexture** pOutTexture, DataBlob& Memory, EDefaultTexture DefaultTexture, bool ForceSRGB );
	void DestroyDefaultTextures();

protected:

	void Initialize();
	void UnInitialize();

	CriticalSection m_Mutex;

protected:
#if R_WITH_D3D12
	std::unordered_map<String, std::unique_ptr<ManagedTexture>> m_TextureCache;
#endif

};
