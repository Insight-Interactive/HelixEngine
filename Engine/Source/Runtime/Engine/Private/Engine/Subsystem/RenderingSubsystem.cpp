#include "EnginePCH.h"

#include "Engine/Subsystem/RenderingSubsystem.h"

#include "App/App.h"
#include "World/Scene.h"
#include "Renderer/SceneRenderer.h"
#include "Engine/ViewportContext.h"
#include "Engine/Window.h"
#include "SwapChain.h"
#include "CommandContext.h"
#include "RendererInitializer.h"


FRenderingSubsystem::FRenderingSubsystem()
	: FSubsystemInterface( "Rendering Subsystem" )
{
}

FRenderingSubsystem::~FRenderingSubsystem()
{

}

void FRenderingSubsystem::Initialize()
{
	m_Scenes.reserve( 1 );
	// Initialize the renderer.
	FRendererInitializer::InitializeContext( m_RenderContext );
}

void FRenderingSubsystem::UnInitialize()
{
	TerminateAsyncProcess();

	FRendererInitializer::UnInitializeContext( m_RenderContext );
}

void FRenderingSubsystem::RunAsync_Implementation()
{
	while (m_IsRunning)
	{
		for (HScene* pScene : m_Scenes)
		{
			HScene& Scene = *pScene;
			if (Scene.IsDirty())
			{
				// Render.
				FSceneRenderParams& SceneRenderParams = Scene.GetRenderParams();
				FColorBuffer& RenderTarget = *SceneRenderParams.pRenderTarget;

				SceneRenderParams.pRenderer->RenderScene(
					Scene,
					RenderTarget, *SceneRenderParams.pView, *SceneRenderParams.pScissor,
					SceneRenderParams.pRenderingViewport->GetWindow().GetSwapChain()->GetCurrentFrameIndex(),
					SceneRenderParams.pRenderingCamera );


				// Present.
				if (SceneRenderParams.FlipSwapChainBuffers)
				{
					// Transition.
					FCommandContext& CmdContext = FCommandContext::Begin( TEXT( "Present" ) );
					{
						CmdContext.TransitionResource( RenderTarget, RS_Present );
					}
					CmdContext.End(true);

					SceneRenderParams.pRenderingViewport->PresentOneFrame();
				}

				Scene.OnRenderingFinished();
			}
		}
	}

	HE_LOG( Log, TEXT( "Exiting rendering async run loop." ) );
}