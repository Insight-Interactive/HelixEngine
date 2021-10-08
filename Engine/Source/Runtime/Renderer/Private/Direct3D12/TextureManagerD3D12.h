#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ITextureManager.h"
#include "TextureD3D12.h"

class RENDER_API HManagedTextureD3D12 : public HManagedTexture, public HTextureD3D12
{
public:
	HManagedTextureD3D12(const String& FileName)
		: HManagedTexture(FileName)
	{
	}
	virtual ~HManagedTextureD3D12()
	{
	}

	virtual void CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool sRGB) override;

};


class RENDER_API FTextureManagerD3D12 : public FTextureManager
{
	friend class FRenderContextFactory;
	friend class D3D12RenderContextFactory;
public:
	FTextureManagerD3D12()
	{
	}
	virtual ~FTextureManagerD3D12()
	{
		UnInitialize();
	}

	virtual HTextureRef LoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual HManagedTexture* FindOrLoadTexture(const String& FileName, EDefaultTexture Fallback, bool forceSRGB) override;
	virtual void DestroyTexture(const String& Key) override;

private:
	std::map<String, std::unique_ptr<HManagedTextureD3D12>> m_TextureCache;
	HTextureD3D12 m_DefaultTextures[DT_NumDefaultTextures];
};
