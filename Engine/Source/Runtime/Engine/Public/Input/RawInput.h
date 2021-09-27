// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/FMouse.h"
#include "Input/FKeyboard.h"


struct PlatformRIDInterface
{
public:
	PlatformRIDInterface() 
	{
	}
	~PlatformRIDInterface() 
	{
	}
	
	bool IsValid() const;

	void Setup();
	void Destroy();
	void* GetInterface();

private:
#if HE_INPUT_USE_XINPUT
	IDirectInput8A* m_pDirectInput = NULL;
#endif
};


/*
	Captures the raw input from the system and tracks the key states per-frame.
*/
class RawInputSurveyer
{
	friend class ViewportContext;
public:
	RawInputSurveyer();
	~RawInputSurveyer();

	/*
		Initialize the input polling system.
		@param pNativeWindow - The window to poll input from.
	*/
	void Initialize( void* pNativeWindow );
	/*
		Shutdown the input polling system and unaquire all devices from the window.
	*/
	void UnInitialize();
	/*
		Poll the window and update the input values.
		@param DeltaTime - The time between frames.
	*/
	void Update( float DeltaTime );
	/*
		Set the window to poll input from.
		@param pNativeWindow - The native window handle to poll input from.
	*/
	void SetWindow( void* pNativeWindow );

	/*
		Returns true if any key is pressed, false if not.
	*/
	bool IsAnyPressed();
	/*
		True if the specified mouse button is pressed, false if not.
	*/
	bool IsMouseButtonPressed( DigitalInput di );
	/*
		True if the specified button is pressed, false if not.
	*/
	bool IsPressed( DigitalInput di );
	/*
		True if the specified button is first pressed, false if not.
	*/
	bool IsFirstPressed( DigitalInput di );
	/*
		True if the specified button is released, false if not.
	*/
	bool IsReleased( DigitalInput di );
	/*
		True if the specified button is first released, false if not.
	*/
	bool IsFirstReleased( DigitalInput di );
	/*
		Returns the time in seconds a key is currently heing held down for.
	*/
	float GetDurationPressed( DigitalInput di );
	/*
		Returns the movement delta for a specified analog input.
	*/
	float GetAnalogInput( DigitalInput ai );
	/*
		Returns the time corrected input for a specified analog input.
	*/
	float GetTimeCorrectedAnalogInput( DigitalInput ai );
	/*
		Aquire all input devices.
	*/
	void AcquireDevices();
	/*
		WARNING: This will unaquire all input methods, leaving no valid input option
		immediatly proceeding this call. It should usually never be used.
	*/
	void UnacquireDevices();
	/*
		Aquire the mouse. Locking it to the center of the screen and disabling
		the visual cursor. Call before expecting valid values from RawInputSurveyer::GetAnalogInput.
	*/
	void AcquireMouse();
	/*
		Unaquire the mouse. Unlocking it from the center of the screen and reqctivating the cursor.
	*/
	void UnacquireMouse();
	/*
		Aquire the keyboard. Allowing for key presses to be polled
	*/
	void AcquireKeyboard();
	/*
		Unaquire the keyboard. Disallowing any key presses to be polled.
	*/
	void UnacquireKeyboard();

	float GetMouseMoveDeltaX();

	float GetMouseMoveDeltaY();


private:
#if HE_INPUT_USE_KEYBOARD_MOUSE
	void KbmInitialize();
	void KbmZeroInputs();
	void KbmShutdown();
	void KbmUpdate();
	void SetMouseMoveDelta( float XValue, float YValue );
	void SetAnalogValue( DigitalInput Input, float Value );

	FMouse m_Mouse;
	FKeyboard m_Keyboard;
	PlatformRIDInterface m_RIDInterface;

#endif // HE_INPUT_USE_KEYBOARD_MOUSE

#if HE_INPUT_USE_GAMEPAD
	float FilterAnalogInput( int Val, int DeadZone );

#endif // HE_INPUT_USE_GAMEPAD

private:
	bool m_UseKeyBoardMouse;

	bool m_Buttons[2][kNumDigitalInputs];
	float m_HoldDuration[kNumDigitalInputs] = { 0.0f };
	float m_Analogs[kNumAnalogInputs];
	float m_AnalogsTC[kNumAnalogInputs];


	void* m_pSurveyingWindow;
};


//
// Inline function implementations
//

// PlatformRIDInterface
//

inline bool PlatformRIDInterface::IsValid() const
{
#if HE_INPUT_USE_XINPUT
	return m_pDirectInput != NULL;
#else
	return false;
#endif
}

inline void PlatformRIDInterface::Setup()
{
#if HE_INPUT_USE_XINPUT
	HRESULT hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // DirectInput8 initialization failed.
#endif
}

inline void PlatformRIDInterface::Destroy()
{
#if HE_INPUT_USE_XINPUT
	if (IsValid())
	{
		m_pDirectInput->Release();
		m_pDirectInput = nullptr;
	}
#endif
}

inline void* PlatformRIDInterface::GetInterface()
{
#if HE_INPUT_USE_XINPUT
	return RCast<void*>(m_pDirectInput);
#else
	return NULL;
#endif
}

// RawInputSurveyer
//

inline void RawInputSurveyer::SetWindow( void* pNativeWindow )
{
	HE_ASSERT( pNativeWindow != NULL ); // Cannot poll input from a null window!
	m_pSurveyingWindow = pNativeWindow;
}

inline float RawInputSurveyer::GetMouseMoveDeltaX()
{
	return m_Mouse.GetMoveDeltaX();
}

inline float RawInputSurveyer::GetMouseMoveDeltaY()
{
	return m_Mouse.GetMoveDeltaY();
}

inline void RawInputSurveyer::SetMouseMoveDelta( float XValue, float YValue )
{
	m_Mouse.SetMouseMoveDelta( XValue, YValue );
}

inline void RawInputSurveyer::SetAnalogValue( DigitalInput Input, float Value )
{
	HE_ASSERT( IsValidAnalogInput( Input ) );
	m_Analogs[Input - AnalogLeftTrigger] = Value;
}

inline void RawInputSurveyer::AcquireDevices()
{
	AcquireMouse();
	AcquireKeyboard();
}

inline void RawInputSurveyer::UnacquireDevices()
{
	UnacquireMouse();
	UnacquireKeyboard();
}

inline void RawInputSurveyer::AcquireMouse()
{
	m_Mouse.Acquire();
}

inline void RawInputSurveyer::UnacquireMouse()
{
	m_Mouse.Unacquire();
}

inline void RawInputSurveyer::AcquireKeyboard()
{
	m_Keyboard.Acquire();
}

inline void RawInputSurveyer::UnacquireKeyboard()
{
	m_Keyboard.Unacquire();
}
