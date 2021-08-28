#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ITextureManager.h"
#include "TextureD3D12.h"

class RENDER_API ManagedTextureD3D12 : public IManagedTexture, public TextureD3D12
{
public:
	ManagedTextureD3D12(const std::string& FileName)
		: IManagedTexture(FileName)
	{
	}
	virtual ~ManagedTextureD3D12() = default;

	virtual void CreateFromMemory(DataBlob memory, EDefaultTexture fallback, bool sRGB) override;

};


class RENDER_API TextureManagerD3D12 : public ITextureManager
{
	friend class IRenderContextFactory;
	friend class D3D12RenderContextFactory;
public:
	TextureManagerD3D12()
	{
	}
	virtual ~TextureManagerD3D12()
	{
		UnInitialize();
	}

	virtual TextureRef LoadTexture(const std::string& FileName, EDefaultTexture Fallback, bool forceSRGB) override;

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual IManagedTexture* FindOrLoadTexture(const std::string& FileName, EDefaultTexture Fallback, bool forceSRGB) override;
	virtual void DestroyTexture(const std::string& Key) override;

private:
	std::map<std::string, std::unique_ptr<ManagedTextureD3D12>> m_TextureCache;
	TextureD3D12 m_DefaultTextures[DT_NumDefaultTextures];
};
