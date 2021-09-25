#include "RendererPCH.h"

#include "RendererInitializer.h"
#include "RenderContext.h"

#if R_WITH_D3D12
#	include "RenderContextFactoryD3D12.h"
#endif


/* static */ void RendererInitializer::InitializeContext( ERenderBackend API, RenderContext& Context )
{
	CreateLogger( GRendererLogger, "Renderer" );

	// Initialize the render context.
	switch (API)
	{
#if R_WITH_D3D12
	case RB_Direct3D12:
	{
		RenderContextFactoryD3D12 Factory;
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

/* static */ void RendererInitializer::UnInitializeContext( RenderContext& Context )
{
	Context.UnInitialize();
}

/* static */ void RendererInitializer::InitializeSwapChain( ISwapChain** pOutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height )
{
	RenderContext* Context = RenderContext::GetInstance();
	switch (Context->GetBackendType())
	{
	case RB_Direct3D11: break;
#if R_WITH_D3D12
	case RB_Direct3D12:
		{
			RenderContextFactoryD3D12 Factory;
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
