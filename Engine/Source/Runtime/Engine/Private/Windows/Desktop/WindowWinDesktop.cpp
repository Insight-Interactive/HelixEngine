// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Window.h"

#include "System.h"
#include "Engine/HEngine.h"

#include "RenderContextFactoryD3D12.h"
#include "RendererInitializer.h"
#include "Input/InputEnums.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Engine/Event/EngineEvent.h"


uint32 Window::SWindowInstanceCount = 0;
CriticalSection Window::SWindowInstanceCounterGuard;

LRESULT CALLBACK WindowProceedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


Window::Window()
	: m_hWindowHandle( NULL )
	, m_WindowMode( WM_Windowed )
{
	ZeroMemory( &m_WindowClassName, sizeof( m_WindowClassName ) );
	ZeroMemory( &m_DebugName, sizeof( m_DebugName ) );
}

Window::Window( const TChar* Title, uint32 Width, uint32 Height, bool bHasTitleBar, bool bShowImmediate, Window* pParent )
	: m_hWindowHandle( NULL )
	, m_WindowMode( WM_Windowed )
{
	ZeroMemory( &m_WindowClassName, sizeof( m_WindowClassName ) );
	ZeroMemory( &m_DebugName, sizeof( m_DebugName ) );

	Description Desc;
	Desc.Title = Title;
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.bHasTitleBar = bHasTitleBar;
	Desc.bShowImmediate = bShowImmediate;
	Desc.pParent = pParent;

	Create( Desc );
}

Window::~Window()
{
	Destroy();
}

void Window::Create( const Window::Description& Desc )
{
	SetLastError( 0 );

	if (IsValid())
	{
		HE_LOG( Warning, TEXT( "Trying to initialize a window that has already been created." ) );
		return;
	}

	m_Desc = Desc;

	HINSTANCE WindowsAppInstance = GetModuleHandle( NULL );


	PrintBuffer( m_WindowClassName, TEXT( "WinClass-%i" ), SWindowInstanceCount );

	CopyMemory( m_DebugName, Desc.Title, lstrlen( Desc.Title ) * sizeof( TChar ) );
	HE_LOG( Log, TEXT( "Registering Windows Desktop window with title:  %s  (Class: %s)" ), Desc.Title ? Desc.Title : TEXT( "Unnamed window" ), m_WindowClassName );

	// Resources
	LPWSTR Cursor = IDC_ARROW;

	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.lpfnWndProc		= WindowProceedure;
	wc.hInstance		= WindowsAppInstance;
	wc.lpszClassName	= m_WindowClassName;
	wc.hCursor			= LoadCursor( NULL, Cursor );
	::RegisterClassEx( &wc );
	DWORD Err = GetLastError();
	if (Err == 0)
	{
		ScopedCriticalSection Guard( SWindowInstanceCounterGuard );
		SWindowInstanceCount++;
	}
	HE_ASSERT( Err == 0 );


	// Assemble a rect to align the window with the center to the screen.
	RECT WindowRect;
	ZeroMemory( &WindowRect, sizeof( RECT ) );
	int centerScreenX = ::GetSystemMetrics( SM_CXSCREEN ) / 2 - m_Desc.Width / 2;
	int centerScreenY = ::GetSystemMetrics( SM_CYSCREEN ) / 2 - m_Desc.Height / 2;
	WindowRect.left = centerScreenX;
	WindowRect.top = centerScreenY + 35;
	WindowRect.right = WindowRect.left + m_Desc.Width;
	WindowRect.bottom = WindowRect.top + m_Desc.Height;
	::AdjustWindowRect( &WindowRect, WS_OVERLAPPEDWINDOW, FALSE );

	m_WindowStyle = m_Desc.bHasTitleBar ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW | WS_VISIBLE;
	HWND hParent = m_Desc.pParent ? (HWND)m_Desc.pParent->GetNativeWindow() : NULL;

	m_hWindowHandle = ::CreateWindowEx(
		NULL,					// Window ExStyles
		m_WindowClassName,		// Window Class
		m_Desc.Title,			// Window Title
		m_WindowStyle,			// Window Style

		WindowRect.left,						// Start X
		WindowRect.top,							// Start Y
		WindowRect.right - WindowRect.left,		// Width
		WindowRect.bottom - WindowRect.top,		// Height

		hParent,				// Parent window
		NULL,					// Menu
		WindowsAppInstance,		// Current Windows program application instance
		this					// Additional application data
	);

	if (m_Desc.bHasTitleBar)
		SetTitle( m_Desc.Title );

	if (Desc.bShowImmediate)
		Show();

	if (m_Desc.bAllowDropFiles)
		DragAcceptFiles( m_hWindowHandle, TRUE );

	CreateSwapChain();

	// Setup the raw input devices. Only needs to be done once.
	static bool RIDInitialized = false;
	if (!RIDInitialized)
	{
		RAWINPUTDEVICE RID = {};
		RID.usUsagePage = HID_USAGE_PAGE_GENERIC;
		RID.usUsage		= HID_USAGE_GENERIC_MOUSE;
		RID.dwFlags		= 0;
		RID.hwndTarget	= NULL;
		if ( !RegisterRawInputDevices( &RID, 1, sizeof( RAWINPUTDEVICE ) ) )
		{
			HE_LOG( Error, TEXT( "Failed to register raw input devices. Error: %s" ), System::GetLastSystemError() );
		}
		RIDInitialized = true;
	}
}

void Window::Destroy()
{
	if (IsValid())
	{
		HE_LOG( Log, TEXT( "Destroying window with name {%s}" ), m_DebugName );

		if (::DestroyWindow( m_hWindowHandle ) != 0)
		{
			// Successfully destroyed the window, invalidate the handle.
			m_hWindowHandle = NULL;
		}
		else
		{
			HE_LOG( Error, TEXT( "Failed to destroy window class with name: %s (Error code: %i)" ), m_DebugName, System::GetLastSystemErrorCode() );
			return;
		}

		// Unregister the class.
		if (::UnregisterClass( m_WindowClassName, NULL ) == 0)
		{
			HE_LOG( Error, TEXT( "Failed to unregister window class with class name: %s (Error code: %i)" ), m_WindowClassName, System::GetLastSystemErrorCode() );
			return;
		}

		HE_SAFE_DELETE_PTR( m_pSwapChain );
	}
}

void Window::Close()
{
	WindowClosedEvent e;
	EmitEvent( e );
}

bool Window::IsValid()
{
	return m_hWindowHandle != NULL;
}

bool Window::IsVisible()
{
	return ::IsWindowVisible( m_hWindowHandle );
}

void Window::Show()
{
	HE_LOG( Log, TEXT( "Showing window with name  %s." ), m_DebugName );
	::ShowWindow( m_hWindowHandle, SW_SHOW );
}

void Window::Hide()
{
	HE_LOG( Log, TEXT( "Hiding window with name  %s." ), m_DebugName );
	::ShowWindow( m_hWindowHandle, SW_HIDE );
}

bool Window::HasFocus()
{
	return ::GetFocus() == m_hWindowHandle;
}

void Window::SetFocus()
{
	HE_LOG( Log, TEXT( "Setting window with name  %s  as focus." ), m_DebugName );

	::SetForegroundWindow( m_hWindowHandle );
	::SetFocus( m_hWindowHandle );
}

void Window::Maximize()
{
	ShowWindow( m_hWindowHandle, SW_MAXIMIZE );
}

void Window::Minimize()
{
	ShowWindow( m_hWindowHandle, SW_MINIMIZE );
}

void Window::PresentOneFrame()
{
	m_pSwapChain->SwapBuffers();
}

bool Window::SetTitle( const TChar* NewTitle )
{
	if (!m_Desc.bHasTitleBar)
	{
		HE_LOG( Warning, TEXT( "Trying to set a title for a window that has no title bar. Attempting to override..." ) );
		return false;
	}

	TCharStrCpy( m_WindowTitle, NewTitle );
	return ::SetWindowText( m_hWindowHandle, m_WindowTitle );
}

void Window::GetTitle( TChar* OutTitleBuffer, uint32 BufferLength ) const
{
	if (HasTitleBar())
	{
		GetWindowText( m_hWindowHandle, OutTitleBuffer, BufferLength );
	}
	else
	{
		OutTitleBuffer = NULL;
		BufferLength = -1;
	}
}

void Window::AllowFileDrops( bool bAllow )
{
	m_Desc.bAllowDropFiles = bAllow;
	DragAcceptFiles( m_hWindowHandle, (BOOL)bAllow );
}

IColorBuffer* Window::GetRenderSurface()
{
	return m_pSwapChain->GetColorBufferForCurrentFrame();
}

void* Window::GetNativeWindow()
{
	return RCast<void*>(m_hWindowHandle);
}

void Window::SetParent( Window* pParent )
{
	HWND hParent = NULL;
	if (pParent != NULL && pParent->IsValid())
	{
		hParent = pParent->m_hWindowHandle;
	}

	::SetParent( m_hWindowHandle, hParent );
}

void Window::OnWindowModeChanged()
{
	switch (GetWindowMode())
	{
	case EWindowMode::WM_FullScreen:
	{
		//
		// Bring us into fullscreen mode.
		//
		HE_LOG( Log, TEXT( "Entering fullscreen mode." ) );

		// Save the old window rect so we can restore it when exiting fullscreen mode.
		::GetWindowRect( m_hWindowHandle, &m_WindowRect );

		// Make the window borderless so that the client area can fill the screen.
		SetWindowLong( m_hWindowHandle, GWL_STYLE, m_WindowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME) );

		// Get the settings of the primary display.
		DEVMODE DevMode = {};
		DevMode.dmSize = sizeof( DEVMODE );
		EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &DevMode );

		RECT FullscreenWindowRect = 
		{
			DevMode.dmPosition.x,
			DevMode.dmPosition.y,
			DevMode.dmPosition.x + (LONG) DevMode.dmPelsWidth,
			DevMode.dmPosition.y + (LONG) DevMode.dmPelsHeight 
		};

		SetWindowPos(
			m_hWindowHandle,
			HWND_TOPMOST,
			FullscreenWindowRect.left,
			FullscreenWindowRect.top,
			FullscreenWindowRect.right,
			FullscreenWindowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE );

		ShowWindow( m_hWindowHandle, SW_MAXIMIZE );

		GetSwapChain()->ToggleFullScreen( !m_pSwapChain->GetIsFullScreenEnabled() );
		
		// Get the new window dimensions.
		uint32 NewWidth( FullscreenWindowRect.right - FullscreenWindowRect.left );
		uint32 NewHeight( FullscreenWindowRect.bottom - FullscreenWindowRect.top );

		// Resize the window's swapchain.
		GetSwapChain()->Resize( NewWidth, NewHeight );

		// Emit the resize event.
		WindowResizeEvent e( NewWidth, NewHeight );
		EmitEvent( e );
	}
	break;
	case EWindowMode::WM_Windowed:
	{
		::SetWindowLong( m_hWindowHandle, GWL_STYLE, m_WindowStyle );

		::SetWindowPos(
			m_hWindowHandle,
			HWND_NOTOPMOST,
			m_WindowRect.left,
			m_WindowRect.top,
			m_WindowRect.right - m_WindowRect.left,
			m_WindowRect.bottom - m_WindowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE );

		::ShowWindow( m_hWindowHandle, SW_NORMAL );

		HE_LOG( Log, TEXT( "Exiting fullscreen mode." ) );
	}
	break;
	}
}

void Window::CreateSwapChain()
{
	RendererInitializer::InitializeSwapChain( &m_pSwapChain, GetNativeWindow(), GetWidth(), GetHeight() );
}

//
// Windows Desktop platform
//


LRESULT CALLBACK WindowProceedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	Window* pWindow = (Window*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams) );
		return 1;
	}
	// ------------------------
	// Window Events
	// ------------------------
#if HE_WITH_EDITOR
	case WM_DROPFILES:
	{
		UINT iFile = 0;
		WCHAR lpszFile[MAX_PATH];
		UINT cch = MAX_PATH;
		UINT ret = DragQueryFile( (HDROP)wParam, iFile, lpszFile, cch );
		
		WindowFileDropEvent e( lpszFile );
		pWindow->EmitEvent( e );
		return 0;
	}
#endif // HE_WITH_EDITOR
	case WM_EXITSIZEMOVE:
	{
		// Get the new window dimensions.
		RECT ClientRect = {};
		GetClientRect( hWnd, &ClientRect );
		uint32 NewWidth ( ClientRect.right - ClientRect.left );
		uint32 NewHeight( ClientRect.bottom - ClientRect.top );

		// Resize the window's swapchain.
		pWindow->GetSwapChain()->Resize( NewWidth, NewHeight );
			
		// Emit the resize event.
		WindowResizeEvent e( NewWidth, NewHeight );
		pWindow->EmitEvent( e );
		
		return 0;
	}
	case WM_KILLFOCUS:
	{
		WindowLostFocusEvent e;
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_SETFOCUS:
	{
		WindowFocusEvent e;
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_CLOSE:
	{
		pWindow->Close();
		return 0;
	}
	// ------------------------
	// Mouse Messages
	// ------------------------
	case WM_INPUT:
	{
		UINT DataSize = 0;
		GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, NULL, &DataSize, sizeof( RAWINPUTHEADER ) );
		
		if (DataSize > 0)
		{
			void* RawData = HE_StackAlloc( DataSize );
			if (GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, RawData, &DataSize, sizeof( RAWINPUTHEADER ) ) == DataSize)
			{
				RAWINPUT* pInputData = RCast<RAWINPUT*>( RawData );
				if (pInputData->header.dwType == RIM_TYPEMOUSE)
				{
					MouseRawPointerMovedEvent e( (float)pInputData->data.mouse.lLastX, (float)pInputData->data.mouse.lLastY );
					pWindow->EmitEvent( e );
				}
			}

		}
		// Cleanup the rids.
		return DefWindowProc( hWnd, uMsg, wParam, lParam );

		return 0;
	}
	// ------------------------
	// Mouse Messages
	// ------------------------
	case WM_MOUSEMOVE:
	{
		MousePositionMovedEvent e( LOWORD( lParam ), HIWORD( lParam ) );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		MouseButtonPressedEvent e( Mouse0 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_LBUTTONUP:
	{
		MouseButtonReleasedEvent e( Mouse0 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		MouseButtonPressedEvent e( Mouse1 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_RBUTTONUP:
	{
		MouseButtonReleasedEvent e( Mouse1 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		MouseButtonPressedEvent e( Mouse2 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_MBUTTONUP:
	{
		MouseButtonReleasedEvent e( Mouse2 );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		float YOffset = GET_WHEEL_DELTA_WPARAM( wParam ) / 120.0f;
		MouseWheelScrolledEvent e( 0.f, YOffset );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_MOUSEHWHEEL:
	{
		float XOffset = GET_WHEEL_DELTA_WPARAM( wParam ) / 120.0f;
		MouseWheelScrolledEvent e( XOffset, 0.f );
		pWindow->EmitEvent( e );
		return 0;
	}
	// ------------------------
	// Key Messages
	// ------------------------
	case WM_KEYDOWN:
	{
		KeyPressedEvent e( (int32)wParam, (int16)lParam );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_KEYUP:
	{
		KeyReleasedEvent e( (int32)wParam );
		pWindow->EmitEvent( e );
		return 0;
	}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
