#pragma once

#include "Renderer/RenderPass.h"

#include "RootSignature.h"
#include "PipelineState.h"


class FRootSignature;
class FPipelineState;
enum EFormat;

class FForwardRenderPass : public FRenderPass
{
public:
	FForwardRenderPass();
	virtual ~FForwardRenderPass();
	
	void Initialize( const FVector2& RenderResolution, const EFormat& RenderTargetFormat, const EFormat& DepthFormat );

	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
	virtual void UnBind( FCommandContext& GfxContext ) override;

	virtual void ReloadPipeline() override;

protected:
	// Pipeline
	FRootSignature m_RS;
	FPipelineState m_PSO;

};