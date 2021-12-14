// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/Technique/PostProcessUber.h"

#include "RootSignature.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "Renderer/ConstantBufferStructures.h"


FPostProcessUber::FPostProcessUber()
{
}

FPostProcessUber::~FPostProcessUber()
{
}

void FPostProcessUber::Initialize( FVector2 RenderResolution )
{
	//m_PostProcessSettings.Create( L"", sizeof( PostProcessSettings ) );
}

void FPostProcessUber::UnInitialize()
{
}

void FPostProcessUber::Bind( FCommandContext& GfxContext, const FRect& Viewrect )
{

}

void FPostProcessUber::UnBind( FCommandContext& GfxContext )
{

}

void FPostProcessUber::ReloadPipeline()
{

}

void FPostProcessUber::Resize( const uint32& Width, const uint32& Height ) 
{

}
