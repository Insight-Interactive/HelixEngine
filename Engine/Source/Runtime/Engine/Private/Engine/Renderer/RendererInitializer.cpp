#include "EnginePCH.h"

#include "Engine/Renderer/RendererInitializer.h"
#include "Engine/Renderer/RenderContext.h"

#if R_WITH_D3D12
#	include "Engine/Renderer/RenderContextFactory.h"
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
