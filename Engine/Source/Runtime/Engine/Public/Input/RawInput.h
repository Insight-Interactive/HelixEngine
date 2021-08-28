// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/InputEnums.h"

/*
    Captures the raw input from the system and tracks the key states per-frame.
*/
class RawInputSurveyer
{
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
	void Update(float DeltaTime);
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
    bool IsMouseButtonPressed(DigitalInput di);
    /*
        True if the specified button is pressed, false if not.
    */
    bool IsPressed(DigitalInput di);
    /*
        True if the specified button is first pressed, false if not.
    */
    bool IsFirstPressed(DigitalInput di);
    /*
        True if the specified button is released, false if not.
    */
    bool IsReleased(DigitalInput di);
    /*
        True if the specified button is first released, false if not.
    */
    bool IsFirstReleased(DigitalInput di);
    /*
        Returns the time in seconds a key is currently heing held down for.
    */
    float GetDurationPressed(DigitalInput di);
    /*
        Returns the movement delta for a specified analog input.
    */
    float GetAnalogInput( DigitalInput ai);
    /*
        Returns the time corrected input for a specified analog input.
    */
    float GetTimeCorrectedAnalogInput( DigitalInput ai);
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

private:
#if HE_INPUT_USE_KEYBOARD_MOUSE
    void KbmInitialize();
    void KbmBuildKeyMapping();
    void KbmZeroInputs();
    void KbmShutdown();
    void KbmUpdate();

    uint8 m_Keybuffer[256];
    uint8 m_DXKeyMapping[NumKeys]; // Map DigitalInput enum to DX key codes 

#endif // HE_INPUT_USE_KEYBOARD_MOUSE

#if HE_INPUT_USE_GAMEPAD
    float FilterAnalogInput( int Val, int DeadZone );

#endif // HE_INPUT_USE_GAMEPAD

private:
    bool m_UseKeyBoardMouse;
    bool m_MouseAcquired;

    bool m_Buttons[2][kNumDigitalInputs];
    float m_HoldDuration[kNumDigitalInputs] = { 0.0f };
    float m_Analogs[kNumAnalogInputs];
    float m_AnalogsTC[kNumAnalogInputs];

    void* m_pSurveyingWindow;

#if HE_WINDOWS

#   if HE_INPUT_USE_XINPUT
       IDirectInput8A* m_pDirectInput;
       IDirectInputDevice8A* m_pKeyboard;
       IDirectInputDevice8A* m_pMouse;

       DIMOUSESTATE2 m_MouseState;

#   endif // HE_INPUT_USE_XINPUT

#endif // HE_WINDOWS

};


//
// Inline function implementations
//

inline void RawInputSurveyer::SetWindow( void* pNativeWindow )
{
    HE_ASSERT( pNativeWindow != NULL ); // Cannot poll input from a null window!
    m_pSurveyingWindow = pNativeWindow;
}
