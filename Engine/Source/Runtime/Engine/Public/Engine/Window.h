// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Containers/TDynamicArray.h"
#include "Callbacks.h"
#include "SwapChain.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"
#include "CriticalSection.h"


class FSwapChain;
class FColorBuffer;

enum EWindowMode
{
	/*
		Standard window mode that can be dragged and resized with the pointer.
	*/
	WM_Windowed		= 1,

	/*
		Resize the window to match the display dimensions exactly. Covering up the title bar and the windows task bar.
	*/
	WM_Borderless	= 2,

	/*
		Put the window in fullscreen allowing the application to take full advantage 
		of the display monitor. Essentially borderles mode, but with slightly better 
		performance on some platforms at the cost of window switchability.
	*/
	WM_FullScreen	= 3,
};

const uint8 kMaxDebugNameLength = 128;
const uint8 kMaxClassNameSize = 16;
const uint8 kMaxWindowTitleLength = 64;

class FWindow : public EventEmitter<void, Event&>
{
	friend class FViewportContext;
	friend class HEditorEngine;
public:
	/*
		Describes a window that can be shown to the user.
		Zero the structure before use.
	*/
	struct Description
	{
		const TChar* Title;
		FResolution Resolution;
		bool bHasTitleBar;
		bool bShowImmediate;
		bool bAllowDropFiles;
		FWindow* pParent;

		Description()
		{
			ZeroMemory( this, sizeof( Description ) );
		}
	};


	FWindow();
	FWindow( const TChar* Title, uint32 Width, uint32 Height, bool bHasTitleBar, bool bShowImmediate, FWindow* pParent );
	virtual ~FWindow();

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

	FColorBuffer& GetRenderSurface();
	void* GetNativeWindow();
	void SetWindowMode( EWindowMode NewMode );
	void SetParent( FWindow* pParent );
	bool SetTitle( const TChar* NewTitle );
	bool DoesAllowFileDrops();
	void AllowFileDrops( bool bAllow );
	uint32 GetWidth() const;
	uint32 GetHeight() const;
	FVector2 GetDimensions() const;
	bool HasTitleBar() const;
	void GetTitle(TChar* OutTitleBuffer, uint32 BufferLength) const;
	FWindow* GetParent();
	EWindowMode GetWindowMode() const;
	void EnableVSync( bool VsyncEnabled );
	bool IsVSyncEnabled() const;
	FSwapChain* GetSwapChain();
	FRect GetRect() const;

protected:
	void OnWindowModeChanged();
	void CreateSwapChain();
	void PresentOneFrame();
	/*
		Associate the mouse with the window. Some platforms 
		need this for mouse input messages.
	*/
	void MakeMoueWindowAssociation();
	static FRect BuildCenterScreenAlignedRect(const FResolution& WindowResolution);

protected:
	Description m_Desc;
	EWindowMode m_WindowMode;
	TChar m_WindowClassName[kMaxClassNameSize];
	TChar m_WindowTitle[kMaxWindowTitleLength];
	TDynamicArray<OutVoidInEWindowModeFn*> m_OnWindowModeChangedCallbacks;
	FSwapChain m_SwapChain;

	TChar m_DebugName[kMaxDebugNameLength];

	static uint32 SWindowInstanceCount;
	static CriticalSection SWindowInstanceCounterGuard;

#ifdef HE_WINDOWS_DESKTOP
	HWND	m_hWindowHandle;
	uint32	m_WindowStyle;
#elif HE_WINDOWS_UNIVERSAL

	enum
	{
		kMouseButton_Left = 0,
		kMouseButton_Right = 1,
		kMouseButton_Middle = 2,
	};
	// Buffer to itendify which mouse buttons are pressed
	// Simply to keep track of their state, NOT to be used in input.
	bool m_MousePressBuffer[3];
	FVector2 m_LastMousePos;
	bool m_IsActiated;
	CoreWindow m_CoreWindow;

	// Callback Functions
	//
	void OnActivated( const CoreWindow& Sender, const WindowActivatedEventArgs& Args );
	void OnWindowClosed( const CoreWindow& Sender, const CoreWindowEventArgs& Args );
	void OnSizeChanged( const CoreWindow& Sender, const WindowSizeChangedEventArgs& Args );
	void OnAppSuspending( const IInspectable& Sender, const SuspendingEventArgs& Args );
	void OnAppResuming( const IInspectable& Sender, const IInspectable& Args );
	void OnKeyDown( const CoreWindow& Sender, const KeyEventArgs& Args );
	void OnKeyUp( const CoreWindow& Sender, const KeyEventArgs& Args );
	void OnMouseButtonPressed( const CoreWindow& Sender, const PointerEventArgs& Args );
	void OnMouseButtonReleased( const CoreWindow& Sender, const PointerEventArgs& Args );
	void OnPointerMoved( const CoreWindow& Sender, const PointerEventArgs& Args );
	void OnMouseWheelMoved( const CoreWindow& Sender, const PointerEventArgs& Args );

#endif
};


//
// Inline function implementations
//

inline bool FWindow::DoesAllowFileDrops()
{
	return m_Desc.bAllowDropFiles;
}

inline uint32 FWindow::GetWidth() const
{
	return m_Desc.Resolution.Width;
}

inline uint32 FWindow::GetHeight() const
{
	return m_Desc.Resolution.Height;
}

inline FVector2 FWindow::GetDimensions() const
{
	return FVector2( float( m_Desc.Resolution.Width ), float( m_Desc.Resolution.Height ) );
}

inline bool FWindow::HasTitleBar() const
{
	return m_Desc.bHasTitleBar;
}

inline FWindow* FWindow::GetParent()
{
	return m_Desc.pParent;
}

inline EWindowMode FWindow::GetWindowMode() const
{
	return m_WindowMode;
}

inline void FWindow::SetWindowMode( EWindowMode NewMode )
{
	HE_ASSERT( IsVisible() );

	if (m_WindowMode == NewMode)
	{
		HE_LOG( Warning, TEXT( "Trying to set window mode with same mode on window: %s" ), m_DebugName );
		return;
	}
	m_WindowMode = NewMode;

	OnWindowModeChanged();
}

inline void FWindow::EnableVSync( bool VsyncEnabled )
{
	m_SwapChain.ToggleVsync( VsyncEnabled );
}

inline bool FWindow::IsVSyncEnabled() const
{
	return m_SwapChain.GetIsVSyncEnabled();
}

inline FSwapChain* FWindow::GetSwapChain()
{
	return &m_SwapChain;
}
