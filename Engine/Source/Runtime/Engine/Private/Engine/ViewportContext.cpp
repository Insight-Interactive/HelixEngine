// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/ViewportContext.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Engine/Event/EngineEvent.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"


FViewportContext::FViewportContext()
	: m_WorldInView( NULL )
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

void FViewportContext::Update( float DeltaTime )
{
	// Update the world inputs.
	m_InputDispatcher.UpdateInputs( DeltaTime );
}

void FViewportContext::Render()
{
	FColorBuffer& SwapChainBackBuffer = *m_Window.GetRenderSurface();

	if (GEngine->GetIsEditorPresent())
	{
		// Render to a intermediate texture for the editor to display.
		RenderWorld( m_SceneRenderTarget );
	}
	else
	{
		// Render directly to the swapchain.
		RenderWorld( SwapChainBackBuffer );
	}
}

void FViewportContext::SetWorld( HWorld* pWorldToView )
{
	m_WorldInView = pWorldToView;
}

void FViewportContext::RenderWorld( FColorBuffer& RenderTarget  )
{
	if (m_WorldInView == NULL) 
		return;

	m_SceneRenderer.RenderScene( 
		*m_WorldInView->GetScene(), 
		RenderTarget, GetClientViewport(), GetClientRect(), m_Window.GetSwapChain()->GetCurrentFrameIndex(),
		m_WorldInView->GetCurrentSceneRenderCamera() );
}

void FViewportContext::InitializeRenderingResources()
{
	FSceneRendererInitParams InitParams;
	ZeroMemory( &InitParams, sizeof( InitParams ) );
	InitParams.RenderingResolution = m_Window.GetDimensions();
	InitParams.BackBufferFormat = m_Window.GetSwapChain()->GetBackBufferFormat();
	m_SceneRenderer.Initialize( InitParams, *this );

	FVector2 RenderResolution( (float)m_Window.GetWidth(), (float)m_Window.GetHeight() );
	EFormat SwapchainFormat = m_Window.GetSwapChain()->GetBackBufferFormat();
	if(GEngine->GetIsEditorPresent())
		m_SceneRenderTarget.Create( L"Pre-Display Buffer", (uint32)RenderResolution.x, (uint32)RenderResolution.y, 1, m_Window.GetSwapChain()->GetBackBufferFormat() );
	
}
void FViewportContext::ReloadRenderPipelines()
{
	m_SceneRenderer.ReloadPipelines();
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

	return false;
}

bool FViewportContext::OnWindowFocus( WindowFocusEvent& e )
{

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

