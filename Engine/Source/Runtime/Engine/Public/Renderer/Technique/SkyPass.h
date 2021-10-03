#pragma once

#include "Renderer/FRenderPass.h"

#include "ModelManager.h"
#include "ITextureManager.h"
#include "HTexture3D.h"


class SkyboxPass : public FRenderPass
{
public:
	SkyboxPass();
	~SkyboxPass();

	void Initialize(EFormat RenderTargetFormat, EFormat DepthBufferFormat);
	void UnInitialize();

	void Bind(ICommandContext& GfxContext, IColorBuffer& RenderTarget, IDepthBuffer& DepthBuffer);
	void UnBind(ICommandContext& GfxContext, IDepthBuffer& DepthBuffer);

	virtual void ReloadPipeline() override;

	//void SetSkyTexture( HTexture3D Texture );

private:
	// Dont use the FRenderPass interface. This pass is a special case.
	virtual void Bind( ICommandContext& GfxContext, const Rect& Viewrect ) override {}
	virtual void UnBind( ICommandContext& GfxContext ) {}

private:
	StaticMeshGeometryRef m_SkyGeometry;
	TextureRef m_SkyDiffuse;

	IRootSignature* m_pRS;
	IPipelineState* m_pPSO;

	EFormat m_RenderTargetFormat;
	EFormat m_DepthTargetFormat;

	//HTexture3D m_SkyTexture;
};
