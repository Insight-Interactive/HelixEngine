// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Gamepad.h"


namespace Input
{
	extern bool m_Buttons[kState_Count][kNumDigitalInputs];
	extern float m_HoldDuration[kNumDigitalInputs];
	extern float m_Analogs[kNumAnalogInputs];
	extern float m_AnalogsTC[kNumAnalogInputs];
	extern FMouse m_Mouse;
	extern FKeyboard m_Keyboard;

	/*
		Initialize the input polling system.
		@param pNativeWindow - The window to poll input from.
	*/
	void Initialize();
	/*
		Shutdown the input polling system and unaquire all devices from the window.
	*/
	void UnInitialize();
	/*
		Poll the window and update the input values.
	*/
	void Update( float DeltaTime );

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
		Aquire the mouse. Locking it to the center of the screen and disabling
		the visual cursor. Call before expecting valid values from Input::GetAnalogInput.
	*/
	void AcquireMouse();

	/*
		Unaquire the mouse. Unlocking it from the center of the screen and reqctivating the cursor.
	*/
	void UnacquireMouse();

	/*
		Show the mouse.
	*/
	void ShowMouse();

	/*
		Hide the mouse.
	*/
	void HideMouse();

	/*
		Get the screen pos of the mouse.
	*/
	FVector2 GetMouseScreenPos();

	/*
		Get the smoothed X delta of the mouse movement .
	*/
	float GetMouseMoveDeltaX();

	/*
		Get the smoothed Y delta of the mouse movement .
	*/
	float GetMouseMoveDeltaY();

	float GetGamepadLeftStickDeltaX();
	float GetGamepadLeftStickDeltaY();

	float GetGamepadRightStickDeltaX();
	float GetGamepadRightStickDeltaY();



	void SetAnalogValue( DigitalInput Input, float Value );
	void SetMouseMoveDelta( float XValue, float YValue );
	void SetMouseScrollDelta( float XScrollDelta, float YScrollDelta );
	void SetMouseButton( uint8 Button, bool IsPressed );
	void SetKey( uint8 Key, bool IsPressed );



	bool GetIsMouseAcquired();

	void KbmZeroInputs();


}


//
// Inline function implementations
//


// Input
//

inline void Input::SetAnalogValue( DigitalInput Input, float Value )
{
	HE_ASSERT( IsValidAnalogInput( Input ) );
	m_Analogs[Input - AnalogLeftTrigger] = Value;
}

inline void Input::SetMouseMoveDelta( float XValue, float YValue )
{
	m_Mouse.SetMouseMoveDelta( XValue, YValue );
}

inline void Input::SetMouseScrollDelta( float XScrollDelta, float YScrollDelta )
{
	m_Mouse.SetMouseScrollDelta( XScrollDelta, YScrollDelta );
}

inline void Input::SetMouseButton( uint8 Button, bool IsPressed )
{
	m_Mouse.SetMouseButton( Button, IsPressed );
}

inline void Input::SetKey( uint8 Key, bool IsPressed )
{
	m_Keyboard.SetKey( Key, IsPressed );
}

inline bool Input::GetIsMouseAcquired()
{
	return m_Mouse.GetIsAcquired();
}

inline FVector2 Input::GetMouseScreenPos()
{
	return m_Mouse.GetPosition();
}

inline float Input::GetMouseMoveDeltaX()
{
	return m_Analogs[AnalogMouseX - AnalogLeftTrigger];
}

inline float Input::GetMouseMoveDeltaY()
{
	return m_Analogs[AnalogMouseY - AnalogLeftTrigger];
}

inline float Input::GetGamepadLeftStickDeltaX()
{
	return m_Analogs[AnalogLeftStickX - AnalogLeftTrigger];
}

inline float Input::GetGamepadLeftStickDeltaY()
{
	return m_Analogs[AnalogLeftStickY - AnalogLeftTrigger];
}

inline float Input::GetGamepadRightStickDeltaX()
{
	return m_Analogs[AnalogRightStickX - AnalogLeftTrigger];
}

inline float Input::GetGamepadRightStickDeltaY()
{
	return m_Analogs[AnalogRightStickY - AnalogLeftTrigger];
}

inline void Input::AcquireMouse()
{
	m_Mouse.Acquire();
}

inline void Input::UnacquireMouse()
{
	m_Mouse.Unacquire();
}

inline void Input::ShowMouse()
{
	m_Mouse.Show();
}

inline void Input::HideMouse()
{
	m_Mouse.Hide();
}
