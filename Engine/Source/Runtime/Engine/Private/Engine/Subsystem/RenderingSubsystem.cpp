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
#include "UI/Panel.h"

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
	m_UIPanels.reserve( 1 );
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
	return;
	while (m_IsRunning)
	{

	}

	HE_LOG( Log, TEXT( "Exiting rendering async run loop." ) );
}