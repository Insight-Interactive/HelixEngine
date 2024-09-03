#include "EnginePCH.h"

#include "Graphics/UIRenderer.h"

#include "Engine/Engine.h"


FUIRenderer::FUIRenderer()
{
}

FUIRenderer::~FUIRenderer()
{
}

void FUIRenderer::Initialize( const FUIRendererInitParams& InitParams, FViewportContext& OwningViewport )
{
	m_pOwningViewport = &OwningViewport;

}

void FUIRenderer::UnInitialize()
{
}

void FUIRenderer::RenderPanel( FUIPanel& Panel, FColorBuffer& RenderTarget, FDepthBuffer& DepthBuffer, const FViewPort& View, const FRect& Scissor, uint32 SwapchainFrameIndex )
{
	FCommandContext& UIContext = FCommandContext::Begin( L"UI Render Panel" );
	{
		UIContext.TransitionResource( RenderTarget, RS_RenderTarget );
		UIContext.RSSetViewPorts( 1, &View );
		UIContext.RSSetScissorRects( 1, &Scissor );

		// Bind the texture heap
		UIContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap() );

		const FColorBuffer* pRTs[] = { &RenderTarget };
		UIContext.OMSetRenderTargets( HE_ARRAYSIZE( pRTs ), pRTs, &DepthBuffer);

		Panel.RenderWidgets( UIContext );
	}
	UIContext.End(true);
}
