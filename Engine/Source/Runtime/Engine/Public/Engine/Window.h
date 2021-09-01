// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Containers/TDynamicArray.h"
#include "Callbacks.h"
#include "ISwapChain.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"

class ISwapChain;
class IColorBuffer;

enum EWindowMode
{
	WM_Borderless = 1,
	WM_Windowed = 2,
	WM_FullScreen = WM_Borderless,
};

const uint8 kMaxDebugNameLength = 64;
const uint8 kMaxClassNameSize = 16;

class Window : public EventEmitter<void, Event&>
{
	friend class ViewportContext;
public:
	/*
		Describes a window that can be shown to the user.
		Zero the structure before use.
	*/
	struct Description
	{
		const TChar* Title;
		uint32 Width;
		uint32 Height;
		bool bHasTitleBar;
		bool bShowImmediate;
		bool bAllowDropFiles;
		Window* pParent;

		Description()
		{
			ZeroMemory( this, sizeof( Description ) );
		}
	};


	Window();
	Window( const TChar* Title, uint32 Width, uint32 Height, bool bHasTitleBar, bool bShowImmediate, Window* pParent );
	~Window();

	/*
		Create the window and register it with the OS.
	*/
	void Create( const Description& Desc );
	/*
		Destroy the window and unregister it with the OS.
	*/
	void Destroy();

	void Close();
	bool IsValid();
	bool IsVisible();
	void Show();
	void Hide();
	bool HasFocus();
	void SetFocus();

	void Maximize();
	void Minimize();

	IColorBuffer* GetRenderSurface();
	void* GetNativeWindow();
	void SetWindowMode( EWindowMode NewMode );
	void SetParent( Window* pParent );
	bool SetTitle( const TChar* NewTitle );
	bool DoesAllowFileDrops();
	void AllowFileDrops( bool bAllow );
	uint32 GetWidth() const;
	uint32 GetHeight() const;
	FVector2 GetDimensions() const;
	bool HasTitleBar() const;
	Window* GetParent();
	void AttachWindowModeListener( OutVoidInEWindowModeFn* pCallback );
	EWindowMode GetWindowMode() const;
	void EnableVSync( bool VsyncEnabled );
	bool IsVSyncEnabled() const;
	ISwapChain* GetSwapChain();

protected:
	void OnWindowModeChanged();
	void CreateSwapChain();
	void PresentOneFrame();

protected:
	Description m_Desc;
	EWindowMode m_WindowMode;
	TChar m_WindowClassName[kMaxClassNameSize];
	TDynamicArray<OutVoidInEWindowModeFn*> m_OnWindowModeChangedCallbacks;
	ISwapChain* m_pSwapChain;

	TChar m_DebugName[kMaxDebugNameLength];

#ifdef HE_WINDOWS_DESKTOP
	HWND	m_hWindowHandle;
	uint32	m_WindowStyle;
	RECT	m_WindowRect;
#elif HE_WINDOWS_UNIVERSAL

#endif
};


//
// Inline function implementations
//

inline bool Window::DoesAllowFileDrops()
{
	return m_Desc.bAllowDropFiles;
}

inline uint32 Window::GetWidth() const
{
	return m_Desc.Width;
}

inline uint32 Window::GetHeight() const
{
	return m_Desc.Height;
}

inline FVector2 Window::GetDimensions() const
{
	return FVector2( float( m_Desc.Width ), float( m_Desc.Height ) );
}

inline bool Window::HasTitleBar() const
{
	return m_Desc.bHasTitleBar;
}

inline Window* Window::GetParent()
{
	return m_Desc.pParent;
}

inline void Window::AttachWindowModeListener( OutVoidInEWindowModeFn* pCallback )
{
	m_OnWindowModeChangedCallbacks.PushBack( pCallback );
}

inline EWindowMode Window::GetWindowMode() const
{
	return m_WindowMode;
}

inline void Window::SetWindowMode( EWindowMode NewMode )
{
	HE_ASSERT( IsVisible() );

	m_WindowMode = NewMode;

	OnWindowModeChanged();

	for (uint64 i = 0; i < m_OnWindowModeChangedCallbacks.Size(); i++)
		m_OnWindowModeChangedCallbacks[i]( m_WindowMode );
}

inline void Window::EnableVSync( bool VsyncEnabled )
{
	m_pSwapChain->ToggleVsync( VsyncEnabled );
}

inline bool Window::IsVSyncEnabled() const
{
	return m_pSwapChain->GetIsVSyncEnabled();
}

inline ISwapChain* Window::GetSwapChain()
{
	return m_pSwapChain;
}
