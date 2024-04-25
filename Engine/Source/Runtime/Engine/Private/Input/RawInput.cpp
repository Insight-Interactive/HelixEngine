// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/RawInput.h"

#include "Engine/Engine.h"


FRawInputSurveyer::FRawInputSurveyer()
	: m_UseKeyBoardMouse( true )
	, m_pSurveyingWindow( NULL )
{
	ZeroMemory( m_Buttons, sizeof( m_Buttons ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );
}

FRawInputSurveyer::~FRawInputSurveyer()
{
}

void FRawInputSurveyer::Initialize( FWindow* pNativeWindow )
{
	SetWindow( pNativeWindow );
	m_pGamepad = m_GamepadManager.CreateGamepad();
#if HE_INPUT_USE_KEYBOARD_MOUSE
	if (m_UseKeyBoardMouse)
	{
		KbmInitialize();
	}
#endif
}

void FRawInputSurveyer::UnInitialize()
{
#if HE_INPUT_USE_KEYBOARD_MOUSE
	if (m_UseKeyBoardMouse)
	{
		KbmShutdown();
	}
#endif
}

void FRawInputSurveyer::Update( float DeltaTime )
{
	CopyMemory( m_Buttons[kState_Previous], m_Buttons[kState_Current], sizeof( m_Buttons[kState_Current] ) );
	ZeroMemory( m_Buttons[kState_Current], sizeof( m_Buttons[kState_Current] ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );


#if HE_INPUT_USE_GAMEPAD
	m_GamepadManager.Tick( DeltaTime );
	
	m_Buttons[kState_Current][AButton]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_A );
	m_Buttons[kState_Current][BButton]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_B );
	m_Buttons[kState_Current][XButton]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_X );
	m_Buttons[kState_Current][YButton]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_Y );
	m_Buttons[kState_Current][DPadUp]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_DPadUp );
	m_Buttons[kState_Current][DPadDown]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_DPadDown );
	m_Buttons[kState_Current][DPadLeft]		= m_pGamepad->IsButtonPressed( FGamepad::kButton_DPadLeft );
	m_Buttons[kState_Current][DPadRight]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_DPadRight );
	m_Buttons[kState_Current][StartButton]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_Start );
	m_Buttons[kState_Current][BackButton]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_Select );
	m_Buttons[kState_Current][LThumbClick]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_LeftThumb );
	m_Buttons[kState_Current][RThumbClick]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_RightThumb );
	m_Buttons[kState_Current][LShoulder]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_LeftShoulder );
	m_Buttons[kState_Current][RShoulder]	= m_pGamepad->IsButtonPressed( FGamepad::kButton_RightShoulder );
	SetAnalogValue( AnalogLeftTrigger, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_LeftTrigger ) );
	SetAnalogValue( AnalogRightTrigger, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_RightTrigger ) );
	SetAnalogValue( AnalogLeftStickX, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_LeftStickX) );
	SetAnalogValue( AnalogLeftStickY, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_LeftStickY ) );
	SetAnalogValue( AnalogRightStickX, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_RightStickX ) );
	SetAnalogValue( AnalogRightStickY, m_pGamepad->GetAnalogValue( FGamepad::kAnalog_RightStickY ) );


	/*XINPUT_STATE newInputState;
	if (ERROR_SUCCESS == XInputGetState( 0, &newInputState ))
	{
		if (newInputState.Gamepad.wButtons & (1 << 0)) m_Buttons[kState_Current][DPadUp] = true;
		if (newInputState.Gamepad.wButtons & (1 << 1)) m_Buttons[kState_Current][DPadDown] = true;
		if (newInputState.Gamepad.wButtons & (1 << 2)) m_Buttons[kState_Current][DPadLeft] = true;
		if (newInputState.Gamepad.wButtons & (1 << 3)) m_Buttons[kState_Current][DPadRight] = true;
		if (newInputState.Gamepad.wButtons & (1 << 4)) m_Buttons[kState_Current][StartButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 5)) m_Buttons[kState_Current][BackButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 6)) m_Buttons[kState_Current][LThumbClick] = true;
		if (newInputState.Gamepad.wButtons & (1 << 7)) m_Buttons[kState_Current][RThumbClick] = true;
		if (newInputState.Gamepad.wButtons & (1 << 8)) m_Buttons[kState_Current][LShoulder] = true;
		if (newInputState.Gamepad.wButtons & (1 << 9)) m_Buttons[kState_Current][RShoulder] = true;
		if (newInputState.Gamepad.wButtons & (1 << 12)) m_Buttons[kState_Current][AButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 13)) m_Buttons[kState_Current][BButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 14)) m_Buttons[kState_Current][XButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 15)) m_Buttons[kState_Current][YButton] = true;

		SetAnalogValue( AnalogLeftTrigger, newInputState.Gamepad.bLeftTrigger / 255.0f );
		SetAnalogValue( AnalogRightTrigger, newInputState.Gamepad.bRightTrigger / 255.0f );
		SetAnalogValue( AnalogLeftStickX, FilterAnalogInput( newInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogLeftStickY, FilterAnalogInput( newInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogRightStickX, FilterAnalogInput( newInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogRightStickY, FilterAnalogInput( newInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) );
	}*/

#else
	//IVectorView<Gamepad^>^ gamepads = Gamepad::Gamepads;
	//if (gamepads->Size != 0)
	//{
	//	IGamepad^ gamepad = gamepads->GetAt( 0 );
	//	GamepadReading reading = gamepad->GetCurrentReading();
	//	uint32_t Buttons = (uint32_t)reading.Buttons;
	//	if (Buttons & (uint32_t)GamepadButtons::DPadUp) m_Buttons[0][kDPadUp] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::DPadDown) m_Buttons[0][kDPadDown] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::DPadLeft) m_Buttons[0][kDPadLeft] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::DPadRight) m_Buttons[0][kDPadRight] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::Menu) m_Buttons[0][kStartButton] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::View) m_Buttons[0][kBackButton] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::LeftThumbstick) m_Buttons[0][kLThumbClick] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::RightThumbstick) m_Buttons[0][kRThumbClick] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::LeftShoulder) m_Buttons[0][kLShoulder] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::RightShoulder) m_Buttons[0][kRShoulder] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::A) m_Buttons[0][kAButton] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::B) m_Buttons[0][kBButton] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::X) m_Buttons[0][kXButton] = true;
	//	if (Buttons & (uint32_t)GamepadButtons::Y) m_Buttons[0][kYButton] = true;

	//	static const float kAnalogStickDeadZone = 0.18f;

	//	m_Analogs[kAnalogLeftTrigger - AnalogLeftTrigger] = (float)reading.LeftTrigger;
	//	m_Analogs[kAnalogRightTrigger - AnalogLeftTrigger] = (float)reading.RightTrigger;
	//	m_Analogs[kAnalogLeftStickX - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.LeftThumbstickX, kAnalogStickDeadZone );
	//	m_Analogs[kAnalogLeftStickY - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.LeftThumbstickY, kAnalogStickDeadZone );
	//	m_Analogs[kAnalogRightStickX - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.RightThumbstickX, kAnalogStickDeadZone );
	//	m_Analogs[kAnalogRightStickY - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.RightThumbstickY, kAnalogStickDeadZone );
	//}

#endif // HE_INPUT_USE_XINPUT

#if HE_INPUT_USE_KEYBOARD_MOUSE
	KbmUpdate();

	for (uint32_t i = 0; i < kNumKeys; ++i)
	{
		m_Buttons[kState_Current][i] = m_Keyboard.GetKey( i ) != 0;
	}

#if HE_WINDOWS
	for (uint32_t i = 0; i < kNumMouseInputs; ++i)
	{
		if (m_Mouse.GetButtonStateByIndex( i ) > 0)
			m_Buttons[kState_Current][Mouse0 + i] = true;
	}
#endif

	const float kMouseSmoothing = 0.0018f;
	FVector2 MouseMoveDelta = m_Mouse.GetMoveDelta();
	SetAnalogValue( AnalogMouseX, MouseMoveDelta.x * kMouseSmoothing );
	SetAnalogValue( AnalogMouseY, MouseMoveDelta.y * kMouseSmoothing );

	float MouseVerticalScrollDelta = m_Mouse.GetVerticalScrollDelta();
	if (MouseVerticalScrollDelta > 0)
		SetAnalogValue( AnalogMouseScroll, 1.f );
	else if (MouseVerticalScrollDelta < 0)
		SetAnalogValue( AnalogMouseScroll, -1.f );

#endif // HE_INPUT_USE_KEYBOARD_MOUSE

	// Update time duration for buttons pressed
	//
	for (uint32_t i = 0; i < kNumDigitalInputs; ++i)
	{
		if (m_Buttons[kState_Current][i])
		{
			if (!m_Buttons[kState_Previous][i])
				m_HoldDuration[i] = 0.0f;
			else
				m_HoldDuration[i] += DeltaTime;
		}
	}

	for (uint32_t i = 0; i < kNumAnalogInputs; ++i)
	{
		m_AnalogsTC[i] = m_Analogs[i] * DeltaTime;
	}
#if HE_INPUT_USE_KEYBOARD_MOUSE
	// Update mouse last so any information can be extracted 
	// from it first, before it clears for the next frame.
	m_Mouse.UpdateBuffers();

#endif
}

bool FRawInputSurveyer::IsMouseButtonPressed( DigitalInput di )
{
	return m_Buttons[kState_Current][di];
}

bool FRawInputSurveyer::IsAnyPressed()
{
	return m_Buttons[kState_Current] != 0;
}

bool FRawInputSurveyer::IsPressed( DigitalInput di )
{
	return m_Buttons[kState_Current][di];
}

bool FRawInputSurveyer::IsFirstPressed( DigitalInput di )
{
	return m_Buttons[kState_Current][di] && !m_Buttons[kState_Previous][di];
}

bool FRawInputSurveyer::IsReleased( DigitalInput di )
{
	return !m_Buttons[kState_Current][di];
}

bool FRawInputSurveyer::IsFirstReleased( DigitalInput di )
{
	return !m_Buttons[kState_Current][di] && m_Buttons[kState_Previous][di];
}

float FRawInputSurveyer::GetDurationPressed( DigitalInput di )
{
	return m_HoldDuration[di];
}

float FRawInputSurveyer::GetAnalogInput( DigitalInput ai )
{
	HE_ASSERT( IsValidAnalogInput( ai ) );
	// m_Analogs only stores values from AnalogLeftTrigger to AnalogMouseScroll 
	// (0 - 8) so put it back in that range.
	return m_Analogs[ai - AnalogLeftTrigger];
}

float FRawInputSurveyer::GetTimeCorrectedAnalogInput( DigitalInput ai )
{
	HE_ASSERT( IsValidAnalogInput( ai ) );
	// m_Analogs only stores values from AnalogLeftTrigger to AnalogMouseScroll 
	// (0 - 8) so put it back in that range.
	return m_AnalogsTC[ai - AnalogLeftTrigger];
}


// Keyboard and mouse functions
//

#if HE_INPUT_USE_KEYBOARD_MOUSE
void FRawInputSurveyer::KbmInitialize()
{
	// Acquire the hardware input interface.
	m_RIDInterface.Setup();

	// Setup the keyboard.
	m_Keyboard.SetupPlatformHIDProvider( m_RIDInterface.GetInterface(), m_pSurveyingWindow->GetNativeWindow() );

	// Setup the mouse.
	m_Mouse.SetupPlatformHIDProvider( m_RIDInterface.GetInterface(), m_pSurveyingWindow->GetNativeWindow() );

	HE_ASSERT( m_Mouse.IsValid() && m_Keyboard.IsValid() );

	AcquireKeyboard();
	KbmZeroInputs();
}

void FRawInputSurveyer::KbmZeroInputs()
{
	m_Mouse.ZeroInput();
	m_Keyboard.ZeroInput();
}

void FRawInputSurveyer::KbmShutdown()
{
	m_Keyboard.Destroy();
	m_Mouse.Destroy();
	m_RIDInterface.Destroy();
}

void FRawInputSurveyer::KbmUpdate()
{
	// Zero out the inputs (to prevent unwanted movements) if the application 
	// is out of focus and hand back the mouse and keyboard to the OS.
	//
	if (!m_pSurveyingWindow->HasFocus())
	{
		KbmZeroInputs();
		m_Mouse.Unacquire();
		m_Keyboard.Unacquire();
	}
	else
	{
		if (m_Mouse.GetIsAcquired())
		{
			m_Mouse.QueryState();
		}
		m_Keyboard.QueryState();
	}
}
#endif //HE_INPUT_USE_KEYBOARD_MOUSE


// Gamepad functions
//

#if HE_INPUT_USE_GAMEPAD
float FRawInputSurveyer::FilterAnalogInput( int Val, int DeadZone )
{
#if HE_INPUT_USE_XINPUT
	if (Val < 0)
	{
		{
			//return (Val > -DeadZone) * (Val + DeadZone) / (32768.0f - DeadZone)
		}
		if (Val > -DeadZone)
			return 0.0f;
		else
			return (Val + DeadZone) / (32768.0f - DeadZone);
	}
	else
	{
		if (Val < DeadZone)
			return 0.0f;
		else
			return (Val - DeadZone) / (32767.0f - DeadZone);
	}
#else
	if (Val < -DeadZone)
		return (Val + DeadZone) / (1.0f - DeadZone);
	else if (Val > DeadZone)
		return (Val - DeadZone) / (1.0f - DeadZone);
	else
		return 0.0f;
#endif // HE_INPUT_USE_XINPUT
	}
#endif // HE_INPUT_USE_GAMEPAD
