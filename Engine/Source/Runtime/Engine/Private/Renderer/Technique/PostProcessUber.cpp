#include "EnginePCH.h"

#include "Renderer/Technique/PostProcessUber.h"

#include "IRootSignature.h"
#include "IPipelineState.h"
#include "IConstantBufferManager.h"
#include "Renderer/ConstantBufferStructures.h"


FPostProcessUber::FPostProcessUber()
{
}

FPostProcessUber::~FPostProcessUber()
{
}

void FPostProcessUber::Initialize( FVector2 RenderResolution )
{
	GConstantBufferManager->CreateConstantBuffer( L"", &m_pPostProcessSettings, sizeof( PostProcessSettings ) );
}

void FPostProcessUber::UnInitialize()
{
	GConstantBufferManager->DestroyConstantBuffer( m_pPostProcessSettings->GetUID() );
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
