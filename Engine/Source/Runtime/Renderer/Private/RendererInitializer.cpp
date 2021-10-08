#include "RendererPCH.h"

#include "RendererInitializer.h"
#include "RenderContext.h"

#if R_WITH_D3D12
#	include "RenderContextFactoryD3D12.h"
#endif


/* static */ void FRendererInitializer::InitializeContext( ERenderBackend API, FRenderContext& Context )
{
	CreateLogger( GRendererLogger, "Renderer" );

	// Initialize the render context.
	switch (API)
	{
#if R_WITH_D3D12
	case RB_Direct3D12:
	{
		FRenderContextFactoryD3D12 Factory;
		Factory.CreateContext( Context );
		break;
	}
#endif
	case RB_Invalid:
	default:
	{
		R_LOG( Error, TEXT( "Trying to create render context with invalid render backend type!" ) );
		HE_ASSERT( false ); 
		break;
	}
	}
}

/* static */ void FRendererInitializer::UnInitializeContext( FRenderContext& Context )
{
	Context.UnInitialize();
}

/* static */ void FRendererInitializer::InitializeSwapChain( FSwapChain** pOutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height )
{
	FRenderContext* Context = FRenderContext::GetInstance();
	switch (Context->GetBackendType())
	{
	case RB_Direct3D11: break;
#if R_WITH_D3D12
	case RB_Direct3D12:
		{
			FRenderContextFactoryD3D12 Factory;
			Factory.CreateSwapChain( pOutSwapChain, pNativeWindow, Width, Height, GCommandManager, GDevice );
			break;
		}
#endif
	case RB_Invalid:
	default:
		{
			R_LOG( Error, TEXT( "Trying to create swapchain with invalid render backend type!" ) );
			HE_ASSERT( false );
			break;
		}
	}
}
