#include "RendererPCH.h"

#include "RendererInitializer.h"
#include "RenderContext.h"

#if R_WITH_D3D12
#	include "RenderContextFactory.h"
#endif


/* static */ void FRendererInitializer::InitializeContext( FRenderContext& Context )
{
	CreateLogger( GRendererLogger, "Renderer" );

	FRenderContextFactory Factory;
	Factory.CreateContext( Context );

}

/* static */ void FRendererInitializer::UnInitializeContext( FRenderContext& Context )
{
	Context.UnInitialize();
}

/* static */ void FRendererInitializer::InitializeSwapChain( FSwapChain& OutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height )
{
	FRenderContext* Context = FRenderContext::GetInstance();

	FRenderContextFactory Factory;
	Factory.CreateSwapChain( OutSwapChain, pNativeWindow, Width, Height, GCommandManager, GGraphicsDevice );
}
