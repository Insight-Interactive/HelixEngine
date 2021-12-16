#include "EnginePCH.h"

#include "Engine/Subsystem/RenderingSubsystem.h"

#include "App/App.h"
#include "World/Scene.h"
#include "Renderer/SceneRenderer.h"
#include "Engine/ViewportContext.h"
#include "Engine/Window.h"
#include "SwapChain.h"
#include "CommandContext.h"


FRenderingSubsystem::FRenderingSubsystem()
	: FSubsystemInterface( "Rendering Subsystem" )
{

}

FRenderingSubsystem::~FRenderingSubsystem()
{

}

void FRenderingSubsystem::Initialize()
{
}

void FRenderingSubsystem::UnInitialize()
{
	TerminateAsyncProcess();
}

void FRenderingSubsystem::RunAsync_Implementation()
{
	while (m_IsRunning)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			HScene& Scene = *m_Scenes[i];
			if (Scene.IsRendering())
			{
				// Render.
				FSceneRenderParams& SceneRenderParams = Scene.GetRenderParams();
				FColorBuffer& RenderTarget = *SceneRenderParams.pRenderTarget;

				SceneRenderParams.pRenderer->RenderScene(
					Scene,
					RenderTarget, *SceneRenderParams.pView, *SceneRenderParams.pScissor,
					SceneRenderParams.pRenderingViewport->GetWindow().GetSwapChain()->GetCurrentFrameIndex(),
					SceneRenderParams.pRenderingCamera );


				// Transition.
				FCommandContext& CmdContext = FCommandContext::Begin( TEXT( "Present" ) );
				{
					CmdContext.TransitionResource( RenderTarget, RS_Present );
				}
				CmdContext.End();

				// Present.
				if (SceneRenderParams.FlipSwapChainBuffers)
					SceneRenderParams.pRenderingViewport->PresentOneFrame();

				Scene.OnRenderingFinished();
			}
		}
	}

	HE_LOG( Log, TEXT( "Exiting physics rendering async run loop." ) );
}