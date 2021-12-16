// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Window.h"

#include "System.h"
#include "Engine/Engine.h"

#include "RendererInitializer.h"
#include "Input/InputEnums.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"
#include "Engine/Event/EngineEvent.h"


uint32 FWindow::SWindowInstanceCount = 0;
CriticalSection FWindow::SWindowInstanceCounterGuard;

LRESULT CALLBACK WindowProceedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


FWindow::FWindow()
	: m_hWindowHandle( NULL )
	, m_WindowMode( WM_Windowed )
{
	ZeroMemory( &m_WindowClassName, sizeof( m_WindowClassName ) );
	ZeroMemory( &m_DebugName, sizeof( m_DebugName ) );
}

FWindow::FWindow( const TChar* Title, uint32 Width, uint32 Height, bool bHasTitleBar, bool bShowImmediate, FWindow* pParent )
	: FWindow()
{
	Description Desc;
	Desc.Title = Title;
	Desc.Resolution.Width = Width;
	Desc.Resolution.Height = Height;
	Desc.bHasTitleBar = bHasTitleBar;
	Desc.bShowImmediate = bShowImmediate;
	Desc.pParent = pParent;

	Create( Desc );
}

FWindow::~FWindow()
{
	Destroy();
}

void FWindow::Create( const FWindow::Description& Desc )
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
	FRect WindowRect = BuildCenterScreenAlignedRect( m_Desc.Resolution );

	m_WindowStyle = m_Desc.bHasTitleBar ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW | WS_VISIBLE;
	HWND hParent = m_Desc.pParent ? *(HWND*)m_Desc.pParent->GetNativeWindow() : NULL;

	m_hWindowHandle = ::CreateWindowEx(
		NULL,					// FWindow ExStyles
		m_WindowClassName,		// FWindow Class
		m_Desc.Title,			// FWindow Title
		m_WindowStyle,			// FWindow Style

		WindowRect.Left,						// Start X
		WindowRect.Top,							// Start Y
		WindowRect.Right - WindowRect.Left,		// Width
		WindowRect.Bottom - WindowRect.Top,		// Height

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

	MakeMoueWindowAssociation();
}

void FWindow::Destroy()
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

		m_SwapChain.UnInitialize();
	}
}

void FWindow::Close()
{
	WindowClosedEvent e;
	EmitEvent( e );
}

bool FWindow::IsValid()
{
	return m_hWindowHandle != NULL;
}

bool FWindow::IsVisible()
{
	return ::IsWindowVisible( m_hWindowHandle );
}

void FWindow::Show()
{
	HE_LOG( Log, TEXT( "Showing window with name  %s." ), m_DebugName );
	::ShowWindow( m_hWindowHandle, SW_SHOW );
}

void FWindow::Hide()
{
	HE_LOG( Log, TEXT( "Hiding window with name  %s." ), m_DebugName );
	::ShowWindow( m_hWindowHandle, SW_HIDE );
}

bool FWindow::HasFocus()
{
	return ::GetFocus() == m_hWindowHandle;
}

void FWindow::SetFocus()
{
	HE_LOG( Log, TEXT( "Setting window with name  %s  as focus." ), m_DebugName );

	::SetForegroundWindow( m_hWindowHandle );
	::SetFocus( m_hWindowHandle );
}

void FWindow::Maximize()
{
	ShowWindow( m_hWindowHandle, SW_MAXIMIZE );
}

void FWindow::Minimize()
{
	ShowWindow( m_hWindowHandle, SW_MINIMIZE );
}

void FWindow::PresentOneFrame()
{
	m_SwapChain.SwapBuffers();
}

bool FWindow::SetTitle( const TChar* NewTitle )
{
	if (!m_Desc.bHasTitleBar)
	{
		HE_LOG( Warning, TEXT( "Trying to set a title for a window that has no title bar. Attempting to override..." ) );
		return false;
	}

	TCharStrCpy( m_WindowTitle, NewTitle );
	return ::SetWindowText( m_hWindowHandle, m_WindowTitle );
}

void FWindow::GetTitle( TChar* OutTitleBuffer, uint32 BufferLength ) const
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

void FWindow::AllowFileDrops( bool bAllow )
{
	m_Desc.bAllowDropFiles = bAllow;
	DragAcceptFiles( m_hWindowHandle, (BOOL)bAllow );
}

FColorBuffer& FWindow::GetRenderSurface()
{
	return m_SwapChain.GetColorBufferForCurrentFrame();
}

void* FWindow::GetNativeWindow()
{
	return (void*)&m_hWindowHandle;
}

void FWindow::SetParent( FWindow* pParent )
{
	HWND hParent = NULL;
	if (pParent != NULL && pParent->IsValid())
	{
		hParent = pParent->m_hWindowHandle;
	}

	::SetParent( m_hWindowHandle, hParent );
}

FRect FWindow::GetRect() const
{
	RECT Rect = {};
	GetWindowRect( m_hWindowHandle, &Rect );
	return FRect{ Rect.left, Rect.top, Rect.right, Rect.bottom };
}

void FWindow::MakeMoueWindowAssociation()
{
	RAWINPUTDEVICE RID = {};
	RID.usUsagePage = HID_USAGE_PAGE_GENERIC;
	RID.usUsage = HID_USAGE_GENERIC_MOUSE;
	RID.dwFlags = 0;
	RID.hwndTarget = NULL;
	if (!RegisterRawInputDevices( &RID, 1, sizeof( RAWINPUTDEVICE ) ))
	{
		HE_LOG( Error, TEXT( "Failed to register raw input devices. Error: %s" ), System::GetLastSystemError() );
	}
}

/*static*/ FRect FWindow::BuildCenterScreenAlignedRect( const FResolution& WindowResolution )
{
	FRect WindowRect = { };
	int CenterScreenX = ::GetSystemMetrics( SM_CXSCREEN ) / 2 - WindowResolution.Width / 2;
	int CenterScreenY = ::GetSystemMetrics( SM_CYSCREEN ) / 2 - WindowResolution.Height / 2;
	WindowRect.Left		= CenterScreenX;
	WindowRect.Top		= CenterScreenY + 35;
	WindowRect.Right	= WindowRect.Left + WindowResolution.Width;
	WindowRect.Bottom	= WindowRect.Top + WindowResolution.Height;

	return WindowRect;
}

void FWindow::OnWindowModeChanged()
{
	switch (GetWindowMode())
	{
	case EWindowMode::WM_Borderless:
		{
			//
			// Bring us into borderless mode.
			//
			HE_LOG( Log, TEXT( "Window \"%s\" Entering borderless mode." ), m_DebugName );

			GetSwapChain()->ToggleFullScreen( true );

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
				DevMode.dmPosition.x + (LONG)DevMode.dmPelsWidth,
				DevMode.dmPosition.y + (LONG)DevMode.dmPelsHeight
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

			break;
		}
	case EWindowMode::WM_FullScreen:
		{
			//
			// Bring us into fullscreen mode.
			//
			HE_LOG( Log, TEXT( "Window \"%s\" Entering fullscreen mode." ), m_DebugName );

			GetSwapChain()->ToggleFullScreen( true );

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
		
			break;
		}
	case EWindowMode::WM_Windowed:
		{
			//
			// Bring us into windowed mode.
			//
			HE_LOG( Log, TEXT( "Window \"%s\" Entering window mode." ), m_DebugName );

			GetSwapChain()->ToggleFullScreen( false );

			::SetWindowLong( m_hWindowHandle, GWL_STYLE, m_WindowStyle );
		
			FRect DefaultRect = FWindow::BuildCenterScreenAlignedRect( GCommonResolutions[k720p] );

			::SetWindowPos(
				m_hWindowHandle,
				HWND_NOTOPMOST,
				DefaultRect.Left,
				DefaultRect.Top,
				DefaultRect.Right  - DefaultRect.Left,
				DefaultRect.Bottom - DefaultRect.Top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE );

		
			::ShowWindow( m_hWindowHandle, SW_NORMAL );
		
			break;
		}
	default:
		break;
	}

	// Resize the window's swapchain.
	// Note: Let WM_SIZE handle the swapchain resize. 

}

void FWindow::CreateSwapChain()
{
	FRendererInitializer::InitializeSwapChain( m_SwapChain, GetNativeWindow(), GetWidth(), GetHeight() );
}

//
// Windows Desktop platform
//

WPARAM MapLeftRightKeys( WPARAM vk, LPARAM lParam )
{
	WPARAM NewVk = vk;

	UINT Scancode = (lParam & 0x00ff0000) >> 16;
	int Extended = (lParam & 0x01000000) != 0;

	switch (vk) {
	case VK_SHIFT:
		NewVk = MapVirtualKey( Scancode, MAPVK_VSC_TO_VK_EX );
		break;
	case VK_CONTROL:
		NewVk = Extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		NewVk = Extended ? VK_RMENU : VK_LMENU;
		break;
	default:
		// not a key we map from generic to left/right specialized
		//  just return it.
		NewVk = vk;
		break;
	}

	return NewVk;
}

LRESULT CALLBACK WindowProceedure( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	FWindow* pWindow = (FWindow*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreate->lpCreateParams) );
		return 1;
	}
	// ------------------------
	// FWindow Events
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
	case WM_SIZE:
	{
		switch (wParam)
		{
		case SIZE_MAXIMIZED:
		{
			WindowMaximizedEvent e;
			pWindow->EmitEvent( e );
			break;
		}
		case SIZE_MINIMIZED:
		{
			WindowMinimizedEvent e;
			pWindow->EmitEvent( e );
			break;
		}
		default:
			break;
		}

		// Get the new window dimensions.
		uint32 NewWidth		= LOWORD( lParam );
		uint32 NewHeight	= HIWORD( lParam );

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
		::GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, NULL, &DataSize, sizeof( RAWINPUTHEADER ) );
		
		if (DataSize > 0)
		{
			void* RawData = HE_StackAlloc( DataSize );
			if (::GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, RawData, &DataSize, sizeof( RAWINPUTHEADER ) ) == DataSize)
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
	}
	// ------------------------
	// Mouse Messages
	// ------------------------
	case WM_MOUSEMOVE:
	{
		MousePositionMovedEvent e( (float)GET_X_LPARAM( lParam ), (float)GET_Y_LPARAM( lParam ) );
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
		WPARAM MappedKey = MapLeftRightKeys( wParam, lParam );
		KeyPressedEvent e( (int32)MappedKey, (int16)lParam );
		pWindow->EmitEvent( e );
		return 0;
	}
	case WM_KEYUP:
	{
		WPARAM MappedKey = MapLeftRightKeys( wParam, lParam );
		KeyReleasedEvent e( (int32)MappedKey );
		pWindow->EmitEvent( e );
		return 0;
	}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
