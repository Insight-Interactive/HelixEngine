// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/ViewportContext.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Engine/Event/EngineEvent.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "UI/Panel.h"
#include "CommandContext.h"


FViewportContext::FViewportContext()
	: m_pWorldInView( nullptr )
	, m_DepthBuffer( 1, 0 )
{
}

FViewportContext::~FViewportContext()
{
}

void FViewportContext::Initialize( const FWindow::Description& WindowDesc )
{
	m_Window.Create( WindowDesc );
	m_Window.GetSwapChain()->SetClearColor( FColor( .25f, 0.f, 1.f ) );
	m_Window.AddListener( this, &FViewportContext::OnEvent );

	m_InputDispatcher.Initialize( &m_Window );

	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;
	m_ViewPort.Width = (float)m_Window.GetWidth();
	m_ViewPort.Height = (float)m_Window.GetHeight();

	m_ScissorRect.Top = 0;
	m_ScissorRect.Left = 0;
	m_ScissorRect.Right = m_Window.GetWidth();
	m_ScissorRect.Bottom = m_Window.GetHeight();


	InitializeRenderingResources();
}

void FViewportContext::Uninitialize()
{
	m_InputDispatcher.UnInitialize();
	m_Window.Destroy();
}

void FViewportContext::Tick( float DeltaTime )
{
	// Update the world inputs.
	m_InputDispatcher.UpdateInputs( DeltaTime );
}

void FViewportContext::Render()
{
	if (GEngine->GetIsEditorPresent())
	{
		// Render to a intermediate texture for the editor to display.
		RenderWorld( m_SceneRenderTarget );
		RenderUI( m_SceneRenderTarget );
	}
	else
	{
		if (m_pWorldInView != nullptr)
		{
			// Render directly to the swapchain.
			FColorBuffer& RenterTarget = m_Window.GetRenderSurface();
			RenderWorld( RenterTarget );
			RenderUI( RenterTarget );

			// Transition.
			FCommandContext& CmdContext = FCommandContext::Begin( TEXT( "Present" ) );
			CmdContext.TransitionResource( RenterTarget, RS_Present );
			CmdContext.End( true );

			PresentOneFrame();
		}
	}
}

void FViewportContext::SetWorld( HWorld* pWorldToView )
{
	m_pWorldInView = pWorldToView;
}

void FViewportContext::RenderWorld( FColorBuffer& RenderTarget  )
{
	if (m_pWorldInView == nullptr) // This viewport is not viewing a world just return.
		return;

	HScene& Scene = m_pWorldInView->GetScene();

	m_SceneRenderer.RenderScene(
		Scene, RenderTarget, 
		GetClientViewport(), GetClientRect(),
		GetWindow().GetSwapChain()->GetCurrentFrameIndex(),
		m_pWorldInView->GetCurrentSceneRenderCamera() );
}

void FViewportContext::RenderUI( FColorBuffer& RenderTarget )
{
	if (m_pWorldInView == nullptr) // This viewport is not viewing a world just return.
		return;

	std::vector<FUIPanel*>& Panels = m_pWorldInView->GetUIPanels();

	for (FUIPanel* pPanel : Panels)
	{
		HE_ASSERT( pPanel != nullptr );

		if (pPanel->IsHidden())
			continue;

		m_UIRenderer.RenderPanel(
			*pPanel, RenderTarget,
			m_SceneRenderer.GetDepthBuffer(),
			GetClientViewport(), GetClientRect(),
			GetWindow().GetSwapChain()->GetCurrentFrameIndex() );
	}
}

void FViewportContext::InitializeRenderingResources()
{
	FVector2 RenderResolution( (float)m_Window.GetWidth(), (float)m_Window.GetHeight() );
	EFormat SwapchainFormat = m_Window.GetSwapChain()->GetBackBufferFormat();
	if (GEngine->GetIsEditorPresent())
		m_SceneRenderTarget.Create( L"Pre-Display Buffer", (uint32)RenderResolution.x, (uint32)RenderResolution.y, 1, m_Window.GetSwapChain()->GetBackBufferFormat() );

	FSceneRendererInitParams SceneParams;
	ZeroMemory( &SceneParams, sizeof( SceneParams ) );
	SceneParams.RenderingResolution = m_Window.GetDimensions();
	SceneParams.BackBufferFormat = m_Window.GetSwapChain()->GetBackBufferFormat();
	m_SceneRenderer.Initialize( SceneParams, *this );

	FUIRendererInitParams UIParams;
	ZeroMemory( &UIParams, sizeof( UIParams ) );
	UIParams.RenderingResolution = m_Window.GetDimensions();
	UIParams.BackBufferFormat = m_Window.GetSwapChain()->GetBackBufferFormat();
	m_UIRenderer.Initialize( UIParams, *this );
}
void FViewportContext::ReloadRenderPipelines()
{
	m_SceneRenderer.ReloadPipelines();
}

FColorBuffer& FViewportContext::GetMainSceneRenderTarget()
{
	if (GEngine->GetIsEditorPresent())
		return m_SceneRenderTarget;
	else
		return GetWindow().GetRenderSurface();
}

//
// Event Processing
//

void FViewportContext::OnEvent( Event& e )
{
	EventDispatcher Dispatcher(e);

	// Window
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &FViewportContext::OnWindowLostFocus );
	Dispatcher.Dispatch<WindowFocusEvent>( this, &FViewportContext::OnWindowFocus );
	Dispatcher.Dispatch<WindowResizeEvent>( this, &FViewportContext::OnWindowResize );

	// Mouse
	Dispatcher.Dispatch<MouseRawPointerMovedEvent>( this, &FViewportContext::OnMouseRawPointerMoved );
	Dispatcher.Dispatch<MouseWheelScrolledEvent>( this, &FViewportContext::OnMouseWheelScrolled );
	Dispatcher.Dispatch<MouseButtonPressedEvent>( this, &FViewportContext::OnMouseButtonPressed );
	Dispatcher.Dispatch<MouseButtonReleasedEvent>( this, &FViewportContext::OnMouseButtonReleased );

	// Keyboard
	Dispatcher.Dispatch<KeyPressedEvent>( this, &FViewportContext::OnKeyPressed );
	Dispatcher.Dispatch<KeyReleasedEvent>( this, &FViewportContext::OnKeyReleased );
}

bool FViewportContext::OnWindowLostFocus( WindowLostFocusEvent& e )
{
	if (!GEngine->GetIsEditorPresent())
	{
		//ShowMouse();
		//UnlockMouseFromScreenCenter();
	}

	return false;
}

bool FViewportContext::OnWindowFocus( WindowFocusEvent& e )
{
	if (!GEngine->GetIsEditorPresent())
	{
		//HideMouse();
		//LockMouseToScreenCenter();
	}

	return false;
}

bool FViewportContext::OnWindowResize( WindowResizeEvent& e )
{
	if (!GEngine->GetIsEditorPresent())
	{
		// Only resize the buffers if we the editor is not present. 
		// Otherwise we are rendering to a texture.
		GCommandManager.IdleGpu();
		m_SceneRenderer.ResizeBuffers( e.GetWidth(), e.GetHeight() );
		m_SceneRenderTarget.Create( L"Pre-Display Buffer", e.GetWidth(), e.GetHeight(), 1, m_Window.GetSwapChain()->GetBackBufferFormat() );

		m_ViewPort.Width		= (float)e.GetWidth();
		m_ViewPort.Height		= (float)e.GetHeight();
		m_ScissorRect.Right		= e.GetWidth();
		m_ScissorRect.Bottom	= e.GetHeight();
	}

	return false;
}

bool FViewportContext::OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseMoveDelta( e.GetX(), e.GetY() );
	return false;
}

bool FViewportContext::OnMouseWheelScrolled( MouseWheelScrolledEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseScrollDelta( e.GetXOffset(), e.GetYOffset() );
	return false;
}

bool FViewportContext::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseButton( e.GetKeyCode() - Mouse0, true );
	return false;
}

bool FViewportContext::OnMouseButtonReleased( MouseButtonReleasedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseButton( e.GetKeyCode() - Mouse0, false );
	return false;
}

bool FViewportContext::OnKeyPressed( KeyPressedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetKey( (uint8)e.GetPlatformKeycode(), true );
	return false;
}

bool FViewportContext::OnKeyReleased( KeyReleasedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetKey( (uint8)e.GetPlatformKeycode(), false );
	return false;
}

