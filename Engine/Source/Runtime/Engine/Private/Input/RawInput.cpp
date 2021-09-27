// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/RawInput.h"

#include "Engine/HEngine.h"


RawInputSurveyer::RawInputSurveyer()
	: m_UseKeyBoardMouse( true )
	, m_pSurveyingWindow( NULL )
{
	ZeroMemory( m_Buttons, sizeof( m_Buttons ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );
}

RawInputSurveyer::~RawInputSurveyer()
{
}

void RawInputSurveyer::Initialize( void* pNativeWindow )
{
	SetWindow( pNativeWindow );

#if HE_INPUT_USE_KEYBOARD_MOUSE
	if (m_UseKeyBoardMouse)
	{
		KbmInitialize();
	}
#endif
}

void RawInputSurveyer::UnInitialize()
{
#if HE_INPUT_USE_KEYBOARD_MOUSE
	if (m_UseKeyBoardMouse)
	{
		KbmShutdown();
	}
#endif
}

void RawInputSurveyer::Update( float DeltaTime )
{
	CopyMemory( m_Buttons[1], m_Buttons[0], sizeof( m_Buttons[0] ) );
	ZeroMemory( m_Buttons[0], sizeof( m_Buttons[0] ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );

	m_Mouse.UpdateBuffers();


#ifdef HE_INPUT_USE_XINPUT
	XINPUT_STATE newInputState;
	if (ERROR_SUCCESS == XInputGetState( 0, &newInputState ))
	{
		if (newInputState.Gamepad.wButtons & (1 << 0)) m_Buttons[0][DPadUp] = true;
		if (newInputState.Gamepad.wButtons & (1 << 1)) m_Buttons[0][DPadDown] = true;
		if (newInputState.Gamepad.wButtons & (1 << 2)) m_Buttons[0][DPadLeft] = true;
		if (newInputState.Gamepad.wButtons & (1 << 3)) m_Buttons[0][DPadRight] = true;
		if (newInputState.Gamepad.wButtons & (1 << 4)) m_Buttons[0][StartButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 5)) m_Buttons[0][BackButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 6)) m_Buttons[0][LThumbClick] = true;
		if (newInputState.Gamepad.wButtons & (1 << 7)) m_Buttons[0][RThumbClick] = true;
		if (newInputState.Gamepad.wButtons & (1 << 8)) m_Buttons[0][LShoulder] = true;
		if (newInputState.Gamepad.wButtons & (1 << 9)) m_Buttons[0][RShoulder] = true;
		if (newInputState.Gamepad.wButtons & (1 << 12)) m_Buttons[0][AButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 13)) m_Buttons[0][BButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 14)) m_Buttons[0][XButton] = true;
		if (newInputState.Gamepad.wButtons & (1 << 15)) m_Buttons[0][YButton] = true;

		SetAnalogValue( AnalogLeftTrigger, newInputState.Gamepad.bLeftTrigger / 255.0f );
		SetAnalogValue( AnalogRightTrigger, newInputState.Gamepad.bRightTrigger / 255.0f );
		SetAnalogValue( AnalogLeftStickX, FilterAnalogInput( newInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogLeftStickY, FilterAnalogInput( newInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogRightStickX, FilterAnalogInput( newInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) );
		SetAnalogValue( AnalogRightStickY, FilterAnalogInput( newInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) );
	}
#else
	IVectorView<Gamepad^>^ gamepads = Gamepad::Gamepads;
	if (gamepads->Size != 0)
	{
		IGamepad^ gamepad = gamepads->GetAt( 0 );
		GamepadReading reading = gamepad->GetCurrentReading();
		uint32_t Buttons = (uint32_t)reading.Buttons;
		if (Buttons & (uint32_t)GamepadButtons::DPadUp) m_Buttons[0][kDPadUp] = true;
		if (Buttons & (uint32_t)GamepadButtons::DPadDown) m_Buttons[0][kDPadDown] = true;
		if (Buttons & (uint32_t)GamepadButtons::DPadLeft) m_Buttons[0][kDPadLeft] = true;
		if (Buttons & (uint32_t)GamepadButtons::DPadRight) m_Buttons[0][kDPadRight] = true;
		if (Buttons & (uint32_t)GamepadButtons::Menu) m_Buttons[0][kStartButton] = true;
		if (Buttons & (uint32_t)GamepadButtons::View) m_Buttons[0][kBackButton] = true;
		if (Buttons & (uint32_t)GamepadButtons::LeftThumbstick) m_Buttons[0][kLThumbClick] = true;
		if (Buttons & (uint32_t)GamepadButtons::RightThumbstick) m_Buttons[0][kRThumbClick] = true;
		if (Buttons & (uint32_t)GamepadButtons::LeftShoulder) m_Buttons[0][kLShoulder] = true;
		if (Buttons & (uint32_t)GamepadButtons::RightShoulder) m_Buttons[0][kRShoulder] = true;
		if (Buttons & (uint32_t)GamepadButtons::A) m_Buttons[0][kAButton] = true;
		if (Buttons & (uint32_t)GamepadButtons::B) m_Buttons[0][kBButton] = true;
		if (Buttons & (uint32_t)GamepadButtons::X) m_Buttons[0][kXButton] = true;
		if (Buttons & (uint32_t)GamepadButtons::Y) m_Buttons[0][kYButton] = true;

		static const float kAnalogStickDeadZone = 0.18f;

		m_Analogs[kAnalogLeftTrigger - AnalogLeftTrigger] = (float)reading.LeftTrigger;
		m_Analogs[kAnalogRightTrigger - AnalogLeftTrigger] = (float)reading.RightTrigger;
		m_Analogs[kAnalogLeftStickX - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.LeftThumbstickX, kAnalogStickDeadZone );
		m_Analogs[kAnalogLeftStickY - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.LeftThumbstickY, kAnalogStickDeadZone );
		m_Analogs[kAnalogRightStickX - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.RightThumbstickX, kAnalogStickDeadZone );
		m_Analogs[kAnalogRightStickY - AnalogLeftTrigger] = FilterAnalogInput( (float)reading.RightThumbstickY, kAnalogStickDeadZone );
	}

#endif // HE_INPUT_USE_XINPUT

#ifdef HE_INPUT_USE_KEYBOARD_MOUSE
	KbmUpdate();

	for (uint32_t i = 0; i < NumKeys; ++i)
	{
		m_Buttons[0][i] = m_Keyboard.GetState( i ) != 0;
	}

#if HE_WINDOWS
	for (uint32_t i = 0; i < 8; ++i)
	{
		if (m_Mouse.GetButtonStateByIndex( i ) > 0) 
			m_Buttons[0][Mouse0 + i] = true;
	}
#endif

	FVector2 MouseMoveDelta = m_Mouse.GetMoveDelta();
	SetAnalogValue( AnalogMouseX, MouseMoveDelta.x * .0018f );
	SetAnalogValue( AnalogMouseY, MouseMoveDelta.y * -.0018f );

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
		if (m_Buttons[0][i])
		{
			if (!m_Buttons[1][i])
				m_HoldDuration[i] = 0.0f;
			else
				m_HoldDuration[i] += DeltaTime;
		}
	}

	for (uint32_t i = 0; i < kNumAnalogInputs; ++i)
	{
		m_AnalogsTC[i] = m_Analogs[i] * DeltaTime;
	}
}

bool RawInputSurveyer::IsMouseButtonPressed( DigitalInput di )
{
#if HE_WINDOWS

	int Key = 0;
	switch (di)
	{
	case Mouse0:
		Key = VK_LBUTTON;
		break;
	case Mouse1:
		Key = VK_RBUTTON;
		break;
	case Mouse2:
		Key = VK_MBUTTON;
		break;
	default:
		HE_LOG( Log, TEXT( "[Input] Invalid mouse key provided when determining weather mouse button was pressed." ) );
		break;
	}
	return ((1 << 15) & ::GetAsyncKeyState( Key ));
	
#endif // HE_WINDOWS
}

bool RawInputSurveyer::IsAnyPressed()
{
	return m_Buttons[0] != 0;
}

bool RawInputSurveyer::IsPressed( DigitalInput di )
{
	return m_Buttons[0][di];
}

bool RawInputSurveyer::IsFirstPressed( DigitalInput di )
{
	return m_Buttons[0][di] && !m_Buttons[1][di];
}

bool RawInputSurveyer::IsReleased( DigitalInput di )
{
	return !m_Buttons[0][di];
}

bool RawInputSurveyer::IsFirstReleased( DigitalInput di )
{
	return !m_Buttons[0][di] && m_Buttons[1][di];
}

float RawInputSurveyer::GetDurationPressed( DigitalInput di )
{
	return m_HoldDuration[di];
}

float RawInputSurveyer::GetAnalogInput( DigitalInput ai )
{
	HE_ASSERT( IsValidAnalogInput( ai ) );
	// m_Analogs only stores values from AnalogLeftTrigger to AnalogMouseScroll 
	// (0 - 8) so put it back in that range.
	return m_Analogs[ai - AnalogLeftTrigger];
}

float RawInputSurveyer::GetTimeCorrectedAnalogInput( DigitalInput ai )
{
	HE_ASSERT( IsValidAnalogInput( ai ) );
	// m_Analogs only stores values from AnalogLeftTrigger to AnalogMouseScroll 
	// (0 - 8) so put it back in that range.
	return m_AnalogsTC[ai - AnalogLeftTrigger];
}

#if HE_INPUT_USE_KEYBOARD_MOUSE
void RawInputSurveyer::KbmInitialize()
{
	// Acquire the hardware input interface.
	m_RIDInterface.Setup();

	// Setup the keyboard.
	m_Keyboard.SetupPlatformRIDProvider( m_RIDInterface.GetInterface(), m_pSurveyingWindow );

	// Setup the mouse.
	m_Mouse.SetupPlatformRIDProvider( m_RIDInterface.GetInterface(), m_pSurveyingWindow );
	
	HE_ASSERT( m_Mouse.IsValid() && m_Keyboard.IsValid() );

	AcquireKeyboard();
	KbmZeroInputs();
}

void RawInputSurveyer::KbmZeroInputs()
{
	m_Mouse.ZeroInput();
	m_Keyboard.ZeroInput();
}

void RawInputSurveyer::KbmShutdown()
{
	m_Keyboard.Destroy();
	m_Mouse.Destroy();
	m_RIDInterface.Destroy();
}

void RawInputSurveyer::KbmUpdate()
{
	// Zero out the inputs if the application is out of focus.
	//
	HWND NativeWindow = RCast<HWND>( m_pSurveyingWindow );
	bool IsVisible = ::IsWindowVisible( NativeWindow ) != 0;
	if (NativeWindow != m_pSurveyingWindow || !IsVisible)
	{
		KbmZeroInputs();
	}
	else
	{
		if ( m_Mouse.GetIsAcquired() )
		{
			AcquireMouse();
			m_Mouse.QueryState();
		}

		AcquireKeyboard();
		m_Keyboard.QueryState();
	}
}
#endif //HE_INPUT_USE_KEYBOARD_MOUSE

#if HE_INPUT_USE_GAMEPAD
float RawInputSurveyer::FilterAnalogInput( int Val, int DeadZone )
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
