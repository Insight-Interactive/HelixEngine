#pragma once

#include "ModelManager.h"
#include "ITextureManager.h"


class SkyboxPass
{
public:
	SkyboxPass();
	~SkyboxPass();

	void Initialize(EFormat RenderTargetFormat, EFormat DepthBufferFormat);
	void UnInitialize();

	void Bind(ICommandContext& GfxContext, IColorBuffer& RenderTarget, IDepthBuffer& DepthBuffer);
	void UnBind(ICommandContext& GfxContext, IDepthBuffer& DepthBuffer);

private:
	StaticMeshGeometryRef m_SkyGeometry;
	TextureRef m_SkyDiffuse;

	IRootSignature* m_pRS;
	IPipelineState* m_pPSO;
};
