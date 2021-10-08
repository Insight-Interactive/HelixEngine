#pragma once

#include "Renderer/RenderPass.h"

#include "ModelManager.h"
#include "ITextureManager.h"
#include "Texture3D.h"


class FSkyboxPass : public FRenderPass
{
public:
	FSkyboxPass();
	~FSkyboxPass();

	void Initialize(EFormat RenderTargetFormat, EFormat DepthBufferFormat);
	void UnInitialize();

	void Bind(FCommandContext& GfxContext, FColorBuffer& RenderTarget, FDepthBuffer& DepthBuffer);
	void UnBind(FCommandContext& GfxContext, FDepthBuffer& DepthBuffer);

	virtual void ReloadPipeline() override;

	//void SetSkyTexture( HTexture3D Texture );

private:
	// Dont use the FRenderPass interface. This pass is a special case.
	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override {}
	virtual void UnBind( FCommandContext& GfxContext ) {}

private:
	StaticMeshGeometryRef m_SkyGeometry;
	HTextureRef m_SkyDiffuse;

	FRootSignature* m_pRS;
	FPipelineState* m_pPSO;

	EFormat m_RenderTargetFormat;
	EFormat m_DepthTargetFormat;

	//HTexture3D m_SkyTexture;
};
