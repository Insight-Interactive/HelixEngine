// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/Technique/ForwardRenderPass.h"

#include "Renderer/ShaderRegisters.h"
#include "Renderer/Common.h"
#include "RendererCore.h"
#include "RenderDevice.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "CommonStructHelpers.h"
#include "CommandContext.h"


FForwardRenderPass::FForwardRenderPass()
{

}

FForwardRenderPass::~FForwardRenderPass()
{

}

void FForwardRenderPass::Initialize( const FVector2& RenderResolution, const EFormat& RenderTargetFormat, const EFormat& DepthFormat )
{

};

void FForwardRenderPass::Bind( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GfxContext.BeginDebugMarker( TEXT( "Forward Render Pass" ) );
}

void FForwardRenderPass::UnBind( FCommandContext& GfxContext ) 
{
	GfxContext.EndDebugMarker();
}

void FForwardRenderPass::ReloadPipeline()
{

}
