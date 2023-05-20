// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Gamepad.h"


class FWindow;


/*
	Some platforms require a base interface for mouse and keyboard input.
	FPlatformHIDInterface is a light wrapper around those interfaces if needed.
*/
struct FPlatformHIDInterface
{
public:
	FPlatformHIDInterface() 
	{
	}
	~FPlatformHIDInterface() 
	{
	}
	
	bool IsValid() const;

	void Setup();
	void Destroy();
	void* GetInterface();

private:

};


/*
	Captures the raw input from the system and tracks the key states per-frame.
*/
class FRawInputSurveyer
{
	friend class FViewportContext;
public:
	FRawInputSurveyer();
	~FRawInputSurveyer();

	/*
		Initialize the input polling system.
		@param pNativeWindow - The window to poll input from.
	*/
	void Initialize( FWindow* pNativeWindow );
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
	void SetWindow( FWindow* pNativeWindow );

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
		the visual cursor. Call before expecting valid values from FRawInputSurveyer::GetAnalogInput.
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

	bool GetIsMouseAcquired();
	bool GetIsKeyboardAcquired();

	void ShowMouse();
	void HideMouse();
	void KbmZeroInputs();

private:
#if HE_INPUT_USE_KEYBOARD_MOUSE
	void KbmInitialize();
	void KbmShutdown();
	void KbmUpdate();
	void SetMouseMoveDelta( float XValue, float YValue );
	void SetMouseScrollDelta( float XScrollDelta, float YScrollDelta );
	void SetMouseButton( uint8 Button, bool IsPressed );
	void SetAnalogValue( DigitalInput Input, float Value );
	void SetKey( uint8 Key, bool IsPressed );

	FMouse m_Mouse;
	FKeyboard m_Keyboard;
	FPlatformHIDInterface m_RIDInterface;

#endif // HE_INPUT_USE_KEYBOARD_MOUSE

#if HE_INPUT_USE_GAMEPAD
	float FilterAnalogInput( int Val, int DeadZone );

#endif // HE_INPUT_USE_GAMEPAD

private:
	bool m_UseKeyBoardMouse;

	bool m_Buttons[kState_Count][kNumDigitalInputs];
	float m_HoldDuration[kNumDigitalInputs] = { 0.0f };
	float m_Analogs[kNumAnalogInputs];
	float m_AnalogsTC[kNumAnalogInputs];

#if HE_INPUT_USE_GAMEPAD
	FGamepadQueryBatcher m_GamepadManager;
	FGamepad* m_pGamepad;
#endif

	FWindow* m_pSurveyingWindow;
};


//
// Inline function implementations
//

// FPlatformHIDInterface
//

inline bool FPlatformHIDInterface::IsValid() const
{
	return false;
}

inline void FPlatformHIDInterface::Setup()
{
}

inline void FPlatformHIDInterface::Destroy()
{
	if (IsValid())
	{

	}
}

inline void* FPlatformHIDInterface::GetInterface()
{
	return NULL;
}

// FRawInputSurveyer
//

inline void FRawInputSurveyer::SetWindow( FWindow* pNativeWindow )
{
	HE_ASSERT( pNativeWindow != NULL ); // Cannot poll input from a null window!
	m_pSurveyingWindow = pNativeWindow;
}

inline bool FRawInputSurveyer::GetIsMouseAcquired()
{
	return m_Mouse.GetIsAcquired();
}

inline bool FRawInputSurveyer::GetIsKeyboardAcquired()
{
	return m_Keyboard.GetIsAcquired();
}


inline float FRawInputSurveyer::GetMouseMoveDeltaX()
{
	return m_Analogs[AnalogMouseX - AnalogLeftTrigger];
	//return m_Mouse.GetMoveDeltaX();
}

inline float FRawInputSurveyer::GetMouseMoveDeltaY()
{
	return m_Analogs[AnalogMouseY - AnalogLeftTrigger];
	//return m_Mouse.GetMoveDeltaY();
}

inline void FRawInputSurveyer::SetMouseMoveDelta( float XValue, float YValue )
{
	m_Mouse.SetMouseMoveDelta( XValue, YValue );
}

inline void FRawInputSurveyer::SetMouseScrollDelta( float XScrollDelta, float YScrollDelta )
{
	m_Mouse.SetMouseScrollDelta( XScrollDelta, YScrollDelta );
}

inline void FRawInputSurveyer::SetMouseButton( uint8 Button, bool IsPressed )
{
	m_Mouse.SetMouseButton( Button, IsPressed );
}

inline void FRawInputSurveyer::SetAnalogValue( DigitalInput Input, float Value )
{
	HE_ASSERT( IsValidAnalogInput( Input ) );
	m_Analogs[Input - AnalogLeftTrigger] = Value;
}

inline void FRawInputSurveyer::SetKey( uint8 Key, bool IsPressed )
{
	m_Keyboard.SetKey( Key, IsPressed );
}

inline void FRawInputSurveyer::AcquireDevices()
{
	AcquireMouse();
	AcquireKeyboard();
}

inline void FRawInputSurveyer::UnacquireDevices()
{
	UnacquireMouse();
	UnacquireKeyboard();
}

inline void FRawInputSurveyer::AcquireMouse()
{
	m_Mouse.Acquire();
}

inline void FRawInputSurveyer::UnacquireMouse()
{
	m_Mouse.Unacquire();
}

inline void FRawInputSurveyer::AcquireKeyboard()
{
	m_Keyboard.Acquire();
}

inline void FRawInputSurveyer::UnacquireKeyboard()
{
	m_Keyboard.Unacquire();
}

inline void FRawInputSurveyer::ShowMouse()
{
	m_Mouse.Show();
}

inline void FRawInputSurveyer::HideMouse()
{
	m_Mouse.Hide();
}
