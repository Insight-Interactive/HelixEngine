#include "EnginePCH.h"

#include "StandaloneRenderer/Technique/PostProcessUber.h"

#include "IRootSignature.h"
#include "IPipelineState.h"
#include "IConstantBufferManager.h"
#include "StandaloneRenderer/ConstantBufferStructures.h"


PostProcesssUber::PostProcesssUber()
{
}

PostProcesssUber::~PostProcesssUber()
{
}

void PostProcesssUber::Initialize( FVector2 RenderResolution )
{
	GConstantBufferManager->CreateConstantBuffer( L"", &m_pPostProcessSettings, sizeof( PostProcessSettings ) );
}

void PostProcesssUber::UnInitialize()
{
	GConstantBufferManager->DestroyConstantBuffer( m_pPostProcessSettings->GetUID() );
}

void PostProcesssUber::Bind( ICommandContext& GfxContext, const Rect& Viewrect )
{

}

void PostProcesssUber::UnBind( ICommandContext& GfxContext )
{

}
