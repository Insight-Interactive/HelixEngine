// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Graphics/RenderPass.h"

#include "Engine/Renderer/RootSignature.h"
#include "Engine/Renderer/PipelineState.h"


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
	virtual void Resize( const uint32& Width, const uint32& Height ) override;

protected:
	// Pipeline
	FRootSignature m_RS;
	FPipelineState m_PSO;

};
