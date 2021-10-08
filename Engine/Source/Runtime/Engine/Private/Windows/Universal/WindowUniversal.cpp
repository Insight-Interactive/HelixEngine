// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Window.h"
#include "System.h"
#include "Engine/Engine.h"
#include "Engine/Event/EngineEvent.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"

#include "RendererInitializer.h"


uint32 FWindow::SWindowInstanceCount = 0;
CriticalSection FWindow::SWindowInstanceCounterGuard;


FWindow::FWindow()
	: m_WindowMode( WM_Windowed )
	, m_IsActiated( false )
	, m_CoreWindow( NULL )
{
	ZeroMemory( m_MousePressBuffer, sizeof( m_MousePressBuffer ) );
}

FWindow::FWindow( const TChar* Title, uint32 Width, uint32 Height, bool bHasTitleBar, bool bShowImmediate, FWindow* pParent )
	: FWindow()
{
	Description Desc;
	Desc.Title = Title;
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.bHasTitleBar = bHasTitleBar;
	Desc.bShowImmediate = bShowImmediate;
	Desc.pParent = pParent;

	Create( Desc );
}

FWindow::~FWindow()
{
	Destroy();
}

void FWindow::Create(const FWindow::Description& Desc)
{
	if (IsValid())
	{
		HE_LOG(Warning, TEXT("Trying to initialize a window that has already been created."));
		return;
	}

	m_Desc = Desc;

	static uint32 WindowInstanceCount = 0;

	PrintBuffer(m_WindowClassName, TEXT("WinClass-%i"), WindowInstanceCount);

	HE_LOG(Log, TEXT("Registering Windows Desktop window with title: %s (Class: %s)"), Desc.Title, m_WindowClassName);

	// Setup the window event callbacks.
	// Window
	m_CoreWindow = CoreWindow::GetForCurrentThread();// CoreApplication::CreateNewView().CoreWindow();
	m_CoreWindow.Activate();
	m_CoreWindow.Activated( { this, &FWindow::OnActivated } );
	m_CoreWindow.Closed( { this, &FWindow::OnWindowClosed } );
	m_CoreWindow.SizeChanged( { this, &FWindow::OnSizeChanged } );
	m_CoreWindow.KeyDown( { this, &FWindow::OnKeyDown } );
	m_CoreWindow.KeyUp( { this, &FWindow::OnKeyUp } );
	m_CoreWindow.PointerPressed( { this, &FWindow::OnMouseButtonPressed } );
	m_CoreWindow.PointerReleased( { this, &FWindow::OnMouseButtonReleased } );
	m_CoreWindow.PointerMoved( { this, &FWindow::OnPointerMoved } );
	m_CoreWindow.PointerWheelChanged( { this, &FWindow::OnMouseWheelMoved } );
	
	// Application
	CoreApplication::Suspending( { this, &FWindow::OnAppSuspending } );
	CoreApplication::Resuming( { this, &FWindow::OnAppResuming } );

	if (m_Desc.bHasTitleBar)
		SetTitle(m_Desc.Title);

	if (Desc.bShowImmediate)
		Show();
	
	CreateSwapChain();

	WindowInstanceCount++;
}

void FWindow::Destroy()
{
	if (IsValid())
	{

	}
}

void FWindow::Close()
{
	WindowClosedEvent e;
	EmitEvent( e );
}

bool FWindow::IsValid()
{
	return m_IsActiated;
}

bool FWindow::IsVisible()
{
	return m_CoreWindow.Visible();
}

void FWindow::Show()
{
}

void FWindow::Hide()
{
}

bool FWindow::HasFocus()
{
	return true;
}

void FWindow::SetFocus()
{

}

void FWindow::Maximize()
{

}

void FWindow::Minimize()
{

}

void FWindow::PresentOneFrame()
{
	m_pSwapChain->SwapBuffers();
}

bool FWindow::SetTitle(const TChar* NewTitle)
{
	if (!m_Desc.bHasTitleBar)
	{
		HE_LOG(Warning, TEXT("Trying to set a title for a window that has no title. Attempting to override..."));
		return false;
	}
	TCharStrCpy( m_WindowTitle, NewTitle );
	ApplicationView::GetForCurrentView().Title( m_WindowTitle );

	return true;
}

void FWindow::GetTitle( TChar* OutTitleBuffer, uint32 BufferLength ) const
{
	winrt::hstring WindowTitle = ApplicationView::GetForCurrentView().Title();
	TCharStrCpy(OutTitleBuffer, WindowTitle.c_str());
}

void FWindow::AllowFileDrops( bool bAllow )
{
	m_Desc.bAllowDropFiles = bAllow;
}

FColorBuffer* FWindow::GetRenderSurface()
{
	return m_pSwapChain->GetColorBufferForCurrentFrame();
}

void* FWindow::GetNativeWindow()
{
	return RCast<void*>( &m_CoreWindow );
}

void FWindow::SetParent(FWindow* pParent)
{
}

void FWindow::OnWindowModeChanged()
{

	switch (GetWindowMode())
	{
	case EWindowMode::WM_FullScreen:
	{
		//
		// Bring us into fullscreen mode.
		//


		HE_LOG(Log, TEXT("Entering fullscreen mode."));

		m_pSwapChain->ToggleFullScreen(!m_pSwapChain->GetIsFullScreenEnabled());
	}
	break;
	case EWindowMode::WM_Windowed:
	{

		HE_LOG(Log, TEXT("Exiting fullscreen mode."));
	}
	break;
	}
}

void FWindow::CreateSwapChain()
{
	FRendererInitializer::InitializeSwapChain( &m_pSwapChain, winrt::get_abi( m_CoreWindow ), GetWidth(), GetHeight() );
}


//
// Windows Universal platform
//

void FWindow::OnActivated( const CoreWindow& Sender, const WindowActivatedEventArgs& Args )
{
	m_IsActiated = true;
}

void FWindow::OnWindowClosed( const CoreWindow& Sender, const CoreWindowEventArgs& Args )
{
	Close();
}

void FWindow::OnSizeChanged( CoreWindow const& Sender, WindowSizeChangedEventArgs const& Args )
{
	// Get the new window dimensions.
	FVector2 NewDims( Sender.Bounds().Width, Sender.Bounds().Height);

	// Resize the window's swapchain.
	m_pSwapChain->Resize( (uint32)NewDims.x, (uint32)NewDims.y );

	// Emit the resize event.
	WindowResizeEvent e( (uint32)NewDims.x, (uint32)NewDims.y );
	EmitEvent( e );
}

void FWindow::OnAppSuspending( const IInspectable& Sender, const SuspendingEventArgs& Args )
{

}

void FWindow::OnAppResuming( const IInspectable& Sender, const IInspectable& Args )
{

}

void FWindow::OnKeyDown( const CoreWindow& Sender, const KeyEventArgs& Args )
{
	auto Key = Args.VirtualKey();
	KeyPressedEvent e( (int32)Key, 0u );
	EmitEvent( e );
}

void FWindow::OnKeyUp( const CoreWindow& Sender, const KeyEventArgs& Args )
{
	auto Key = Args.VirtualKey();
	KeyReleasedEvent e( (int32)Key );
	EmitEvent( e );
}

void FWindow::OnMouseButtonPressed( const CoreWindow& Sender, const PointerEventArgs& Args )
{
	auto PointerProps = Args.CurrentPoint().Properties();

	if (!m_MousePressBuffer[kMouseButton_Left] && PointerProps.IsLeftButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Left] = true;
		MouseButtonPressedEvent e( Mouse0 );
		EmitEvent( e );
	}
	
	if (!m_MousePressBuffer[kMouseButton_Right] && PointerProps.IsRightButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Right] = true;
		MouseButtonPressedEvent e( Mouse1 );
		EmitEvent( e );
	}

	if (!m_MousePressBuffer[kMouseButton_Middle] && PointerProps.IsMiddleButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Middle] = true;
		MouseButtonPressedEvent e( Mouse2 );
		EmitEvent( e );
	}
}

void FWindow::OnMouseButtonReleased( const CoreWindow& Sender, const PointerEventArgs& Args )
{
	auto PointerProps = Args.CurrentPoint().Properties();

	if (m_MousePressBuffer[kMouseButton_Left] && !PointerProps.IsLeftButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Left] = false;
		MouseButtonPressedEvent e( Mouse0 );
		EmitEvent( e );
	}

	if (m_MousePressBuffer[kMouseButton_Right] && !PointerProps.IsRightButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Right] = false;
		MouseButtonPressedEvent e( Mouse1 );
		EmitEvent( e );
	}

	if (m_MousePressBuffer[kMouseButton_Middle] && !PointerProps.IsMiddleButtonPressed())
	{
		m_MousePressBuffer[kMouseButton_Middle] = false;
		MouseButtonPressedEvent e( Mouse2 );
		EmitEvent( e );
	}
}

void FWindow::OnPointerMoved( const CoreWindow& Sender, const PointerEventArgs& Args )
{
	auto CurPoint = Args.CurrentPoint().Position();
	FVector2 CurrentPoint( CurPoint.X, CurPoint.Y );

	// Pointer position
	{
		MousePositionMovedEvent e( CurPoint.X, CurPoint.Y );
		EmitEvent( e );
	}

	// Raw Position
	{
		float X = CurrentPoint.x - m_LastMousePos.x;
		float Y = CurrentPoint.y - m_LastMousePos.y;
		MouseRawPointerMovedEvent e( X, Y );
		EmitEvent( e );
	}

	m_LastMousePos = CurrentPoint;
}

void FWindow::OnMouseWheelMoved( const CoreWindow& Sender, const PointerEventArgs& Args )
{
	auto PointerProps = Args.CurrentPoint().Properties();
	MouseWheelScrolledEvent e( 0.f, (float)PointerProps.MouseWheelDelta() );
	EmitEvent( e );
}
