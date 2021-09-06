// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


class FMouse
{
	friend class RawInputSurveyer;
public:
	FMouse();
	~FMouse();
	
	bool IsValid() const;

	void Acquire();
	void Unacquire();
	void QueryState();
	void UpdateBuffers();
	void ZeroInput();

	bool GetIsAcquired() const;
	float GetMoveDeltaX() const;
	float GetMoveDeltaY() const;
	FVector2 GetMoveDelta() const;
	float GetVerticalScrollDelta() const;
	float GetButtonStateByIndex( const uint8& Index ) const;

private:
	void SetIsAcquired( const bool& Value );
	void SetMouseMoveDelta( const float& XValue, const float& YValue );
	void SetupPlatformRIDProvider( void* pProviderInterface, void* pNativeWindow );
	void Destroy();

private:
	enum
	{
		kMouseDeltaX,
		kMouseDeltaY,

		NumMouseDeltas,
	};
	float m_MouseMoveDelta[2][NumMouseDeltas];
	bool m_IsAcquired;

#if HE_INPUT_USE_XINPUT
	IDirectInputDevice8A* m_pDIDMouse;
	DIMOUSESTATE2 m_State;
#endif

};


//
// Inline function implementations
//

inline void FMouse::Destroy()
{
	if ( IsValid() )
	{
#if HE_INPUT_USE_XINPUT
		m_pDIDMouse->Unacquire();
		m_pDIDMouse->Release();
		m_pDIDMouse = NULL;
#endif
	}
}

inline bool FMouse::IsValid() const
{
#if HE_INPUT_USE_XINPUT
	return m_pDIDMouse != NULL;
#else
	return false;
#endif
}

inline void FMouse::QueryState()
{
#if HE_INPUT_USE_XINPUT
	m_pDIDMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &m_State );
#endif
}

inline void FMouse::UpdateBuffers()
{
	CopyMemory( m_MouseMoveDelta[1], m_MouseMoveDelta[0], sizeof( m_MouseMoveDelta[0] ) );
	ZeroMemory( m_MouseMoveDelta[0], sizeof( m_MouseMoveDelta[0] ) );
}

inline void FMouse::ZeroInput()
{
#if HE_INPUT_USE_XINPUT
	ZeroMemory( &m_State, sizeof( DIMOUSESTATE2 ) );
#endif
}

inline FVector2 FMouse::GetMoveDelta() const
{
	return FVector2( (float)m_State.lX, (float)m_State.lY );
}

inline float FMouse::GetVerticalScrollDelta() const
{
	return (float)m_State.lZ;
}

inline void FMouse::Acquire()
{
	SetIsAcquired( true );

#if HE_INPUT_USE_XINPUT
	m_pDIDMouse->Acquire();
#endif
}

inline void FMouse::Unacquire()
{
	SetIsAcquired( false );

#if HE_INPUT_USE_XINPUT
	m_pDIDMouse->Unacquire();
#endif
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
	return m_MouseMoveDelta[1][kMouseDeltaX];
}

inline float FMouse::GetMoveDeltaY() const
{
	return m_MouseMoveDelta[1][kMouseDeltaY];
}

inline void FMouse::SetMouseMoveDelta( const float& XValue, const float& YValue )
{
	m_MouseMoveDelta[0][kMouseDeltaX] = XValue;
	m_MouseMoveDelta[0][kMouseDeltaY] = YValue;
}

inline float FMouse::GetButtonStateByIndex( const uint8& Index ) const
{
#if HE_INPUT_USE_XINPUT
	HE_ASSERT( Index >= 0 && Index <= 8 );
	return m_State.rgbButtons[Index];
#endif
}
