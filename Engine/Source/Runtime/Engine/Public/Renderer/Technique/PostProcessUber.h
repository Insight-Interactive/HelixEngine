// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Renderer/RenderPass.h"

#include "RootSignature.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"


enum EFormat;

class FPostProcessUber : public FRenderPass
{
public:
	FPostProcessUber();
	virtual ~FPostProcessUber();
	
	void Initialize( FVector2 RenderResolution );
	void UnInitialize();

	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
	virtual void UnBind( FCommandContext& GfxContext ) override;

	virtual void ReloadPipeline() override;
	virtual void Resize( const uint32& Width, const uint32& Height ) override;

protected:

	// Pipeline
	//FRootSignature m_RS;
	//FPipelineState m_PSO;

	//TConstantBuffer<PostProcessSettings> m_PostProcessSettings;

};