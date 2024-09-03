// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Graphics/Technique/ForwardRenderPass.h"


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

void FForwardRenderPass::Resize( const uint32& Width, const uint32& Height ) 
{

}
