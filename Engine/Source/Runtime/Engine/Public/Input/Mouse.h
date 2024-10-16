// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/InputEnums.h"

/*
	Encapsulates a mouse object. If no HID provider is found, relies on window messages to fill buffers.
*/
class FMouse
{
	friend class FRawInputSurveyer;
public:
	FMouse();
	~FMouse();

	bool IsValid() const;

	void Acquire();
	void Unacquire();
	void QueryState();
	void UpdateBuffers();
	void ZeroInput();
	void SetToScreenCenter();

	void Show() const;
	void Hide() const;

	bool GetIsAcquired() const;
	float GetMoveDeltaX() const;
	float GetMoveDeltaY() const;
	FVector2 GetMoveDelta() const;
	float GetVerticalScrollDelta() const;
	float GetButtonStateByIndex( const uint8& Index ) const;

private:
	void SetIsAcquired( const bool& Value );
	void SetMouseMoveDelta( const float& XValue, const float& YValue );
	void SetMouseScrollDelta( const float& XScrollDelta, const float& YScrollDelta );
	void SetMouseButton( const uint8& Button, const bool& Pressed );
	void SetupPlatformHIDProvider( void* pProviderInterface, void* pNativeWindow );
	void Destroy();

private:

	enum
	{
		kMouseDeltaX,
		kMouseDeltaY,
		kMouseDeltaScrollX,
		kMouseDeltaScrollY,

		MouseDeltas_Count,
	};
	struct FMouseState
	{
		// Mouse left (0), right (1), and middle (2).
		bool Buttons[3];
		float MoveDelta[kState_Count][MouseDeltas_Count];
	};
	bool m_IsAcquired;
	FMouseState m_MouseState;
	FVector2 m_ScreenCenter;

};


//
// Inline function implementations
//

inline void FMouse::Destroy()
{
	if (IsValid())
	{
		Unacquire();
	}
}

inline bool FMouse::IsValid() const
{
	return true;
}

inline void FMouse::QueryState()
{
#if HE_WINDOWS_DESKTOP
	// Do Nothing ...

#endif
}

inline void FMouse::UpdateBuffers()
{
	CopyMemory( m_MouseState.MoveDelta[kState_Previous], m_MouseState.MoveDelta[kState_Current], sizeof( m_MouseState.MoveDelta[kState_Current] ) );
	ZeroMemory( m_MouseState.MoveDelta[kState_Current], sizeof( m_MouseState.MoveDelta[kState_Current] ) );

	if (GetIsAcquired())
	{
		SetToScreenCenter();
	}
}

inline void FMouse::ZeroInput()
{
	ZeroMemory( m_MouseState.MoveDelta, sizeof( m_MouseState.MoveDelta ) );
}

inline void FMouse::SetToScreenCenter()
{
	// Calculate the coordinates from the very center of the screen.
#if HE_WINDOWS_UNIVERSAL
	Rect rcClip = CoreWindow::GetForCurrentThread().Bounds();
	m_ScreenCenter.x = rcClip.X + (rcClip.Width * 0.5f);
	m_ScreenCenter.y = rcClip.Y + (rcClip.Height * 0.5f);
	CoreWindow::GetForCurrentThread().PointerPosition( Point( m_ScreenCenter.x, m_ScreenCenter.y ) );

#elif HE_WINDOWS_DESKTOP
	HWND Window = GetFocus();
	RECT rcClip = { 0 };
	::GetWindowRect( Window, &rcClip );

	FVector2 WindowDims = FVector2( float( rcClip.right - rcClip.left ), float( rcClip.bottom - rcClip.top ) );
	m_ScreenCenter.x = rcClip.left + (WindowDims.x * 0.5f);
	m_ScreenCenter.y = rcClip.top + (WindowDims.y * 0.5f);
	::SetCursorPos( (int)m_ScreenCenter.x, (int)m_ScreenCenter.y );

#endif
}

inline void FMouse::Show() const
{
#if HE_WINDOWS_UNIVERSAL
	CoreWindow::GetForCurrentThread().PointerCursor( CoreCursor( CoreCursorType::Arrow, 0 ) );

#elif HE_WINDOWS_DESKTOP
	ShowCursor( TRUE );

#endif
}

inline void FMouse::Hide() const
{
#if HE_WINDOWS_UNIVERSAL
	CoreWindow::GetForCurrentThread().PointerCursor( nullptr );

#elif HE_WINDOWS_DESKTOP
	ShowCursor( FALSE );

#endif
}

inline FVector2 FMouse::GetMoveDelta() const
{
	return FVector2( m_MouseState.MoveDelta[kState_Current][kMouseDeltaX], m_MouseState.MoveDelta[kState_Current][kMouseDeltaY] );
}

inline float FMouse::GetVerticalScrollDelta() const
{
	return m_MouseState.MoveDelta[kState_Current][kMouseDeltaScrollX];
}

inline void FMouse::Acquire()
{
	if (!GetIsAcquired())
	{
		SetIsAcquired( true );
		Hide();
	}
}

inline void FMouse::Unacquire()
{
	if (GetIsAcquired())
	{
		SetIsAcquired( false );
		Show();
	}
}

inline bool FMouse::GetIsAcquired() const
{
	return m_IsAcquired;
}

inline void FMouse::SetIsAcquired( const bool& Value )
{
	m_IsAcquired = Value;
}

inline float FMouse::GetMoveDeltaX() const
{
	return m_MouseState.MoveDelta[kState_Previous][kMouseDeltaX];
}

inline float FMouse::GetMoveDeltaY() const
{
	return m_MouseState.MoveDelta[kState_Previous][kMouseDeltaY];
}

inline void FMouse::SetMouseMoveDelta( const float& XValue, const float& YValue )
{
	m_MouseState.MoveDelta[kState_Current][kMouseDeltaX] = XValue;
	m_MouseState.MoveDelta[kState_Current][kMouseDeltaY] = YValue;
}

inline void FMouse::SetMouseScrollDelta( const float& XScrollDelta, const float& YScrollDelta )
{
	m_MouseState.MoveDelta[kState_Current][kMouseDeltaScrollX] = XScrollDelta;
	m_MouseState.MoveDelta[kState_Current][kMouseDeltaScrollY] = YScrollDelta;
}

inline void FMouse::SetMouseButton( const uint8& Button, const bool& IsPressed )
{
	HE_ASSERT( Button < 3 );
	m_MouseState.Buttons[Button] = IsPressed;
}

inline float FMouse::GetButtonStateByIndex( const uint8& Index ) const
{
	HE_ASSERT( Index >= 0 && Index <= 8 );
	return (float)m_MouseState.Buttons[Index];
}
