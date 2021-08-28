// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/RawInput.h"

#include "Engine/HEngine.h"


RawInputSurveyer::RawInputSurveyer()
	: m_MouseAcquired( false )
	, m_UseKeyBoardMouse( true )
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
	memcpy( m_Buttons[1], m_Buttons[0], sizeof( m_Buttons[0] ) );
	memset( m_Buttons[0], 0, sizeof( m_Buttons[0] ) );
	memset( m_Analogs, 0, sizeof( m_Analogs ) );

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

		m_Analogs[AnalogLeftTrigger] = newInputState.Gamepad.bLeftTrigger / 255.0f;
		m_Analogs[AnalogRightTrigger] = newInputState.Gamepad.bRightTrigger / 255.0f;
		m_Analogs[AnalogLeftStickX] = FilterAnalogInput( newInputState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		m_Analogs[AnalogLeftStickY] = FilterAnalogInput( newInputState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		m_Analogs[AnalogRightStickX] = FilterAnalogInput( newInputState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
		m_Analogs[AnalogRightStickY] = FilterAnalogInput( newInputState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
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

		m_Analogs[kAnalogLeftTrigger] = (float)reading.LeftTrigger;
		m_Analogs[kAnalogRightTrigger] = (float)reading.RightTrigger;
		m_Analogs[kAnalogLeftStickX] = FilterAnalogInput( (float)reading.LeftThumbstickX, kAnalogStickDeadZone );
		m_Analogs[kAnalogLeftStickY] = FilterAnalogInput( (float)reading.LeftThumbstickY, kAnalogStickDeadZone );
		m_Analogs[kAnalogRightStickX] = FilterAnalogInput( (float)reading.RightThumbstickX, kAnalogStickDeadZone );
		m_Analogs[kAnalogRightStickY] = FilterAnalogInput( (float)reading.RightThumbstickY, kAnalogStickDeadZone );
	}

#endif // HE_INPUT_USE_XINPUT

#ifdef HE_INPUT_USE_KEYBOARD_MOUSE
	KbmUpdate();

	for (uint32_t i = 0; i < NumKeys; ++i)
	{
		m_Buttons[0][i] = (m_Keybuffer[m_DXKeyMapping[i]] & 0x80) != 0;
	}

	for (uint32_t i = 0; i < 8; ++i)
	{
		if (m_MouseState.rgbButtons[i] > 0) m_Buttons[0][Mouse0 + i] = true;
	}

	m_Analogs[AnalogMouseX] = (float)m_MouseState.lX * .0018f;
	m_Analogs[AnalogMouseY] = (float)m_MouseState.lY * -.0018f;

	if (m_MouseState.lZ > 0)
		m_Analogs[AnalogMouseScroll] = 1.0f;
	else if (m_MouseState.lZ < 0)
		m_Analogs[AnalogMouseScroll] = -1.0f;

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

bool RawInputSurveyer::IsAnyPressed( void )
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
	return m_Analogs[ai];
}

float RawInputSurveyer::GetTimeCorrectedAnalogInput( DigitalInput ai )
{
	HE_ASSERT( IsValidAnalogInput( ai ) );
	return m_AnalogsTC[ai];
}

void RawInputSurveyer::AcquireDevices()
{
	m_pMouse->Acquire();
	m_pKeyboard->Acquire();
}

void RawInputSurveyer::UnacquireDevices()
{
	m_pMouse->Unacquire();
	m_pKeyboard->Unacquire();
}

void RawInputSurveyer::AcquireMouse()
{
	m_MouseAcquired = true;
	m_pMouse->Acquire();
}

void RawInputSurveyer::UnacquireMouse()
{
	m_MouseAcquired = false;
	m_pMouse->Unacquire();
}

void RawInputSurveyer::AcquireKeyboard()
{
	m_pKeyboard->Acquire();
}

void RawInputSurveyer::UnacquireKeyboard()
{
	m_pKeyboard->Unacquire();
}

#if HE_INPUT_USE_KEYBOARD_MOUSE

void RawInputSurveyer::KbmInitialize()
{
#if HE_WIN64 || HE_WIN32

	HWND NativeWindow = RCast<HWND>( m_pSurveyingWindow );

	KbmBuildKeyMapping();

	HRESULT hr = DirectInput8Create( GetModuleHandle( nullptr ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // DirectInput8 initialization failed.

	hr = m_pDirectInput->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard CreateDevice failed.

	hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard SetDataFormat failed.
	
	hr = m_pKeyboard->SetCooperativeLevel( NativeWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard SetCooperativeLevel failed.

	DIPROPDWORD dipdw = {};
	dipdw.diph.dwSize = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 10;
	hr = m_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	HE_ASSERT( SUCCEEDED( hr ) ); // Keyboard set buffer size failed.

	hr = m_pDirectInput->CreateDevice( GUID_SysMouse, &m_pMouse, nullptr );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse CreateDevice failed.

	hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse SetDataFormat failed.

	hr = m_pMouse->SetCooperativeLevel( NativeWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
	HE_ASSERT( SUCCEEDED( hr ) ); // Mouse SetCooperativeLevel failed.

	AcquireDevices();
	KbmZeroInputs();

#endif // HE_WIN64 || HE_WIN32
}

void RawInputSurveyer::KbmBuildKeyMapping()
{
	m_DXKeyMapping[Key_Escape] = 1;
	m_DXKeyMapping[Key_1] = 2;
	m_DXKeyMapping[Key_2] = 3;
	m_DXKeyMapping[Key_3] = 4;
	m_DXKeyMapping[Key_4] = 5;
	m_DXKeyMapping[Key_5] = 6;
	m_DXKeyMapping[Key_6] = 7;
	m_DXKeyMapping[Key_7] = 8;
	m_DXKeyMapping[Key_8] = 9;
	m_DXKeyMapping[Key_9] = 10;
	m_DXKeyMapping[Key_0] = 11;
	m_DXKeyMapping[Key_Minus] = 12;
	m_DXKeyMapping[Key_Equals] = 13;
	m_DXKeyMapping[Key_Back] = 14;
	m_DXKeyMapping[Key_Tab] = 15;
	m_DXKeyMapping[Key_Q] = 16;
	m_DXKeyMapping[Key_W] = 17;
	m_DXKeyMapping[Key_E] = 18;
	m_DXKeyMapping[Key_R] = 19;
	m_DXKeyMapping[Key_T] = 20;
	m_DXKeyMapping[Key_Y] = 21;
	m_DXKeyMapping[Key_U] = 22;
	m_DXKeyMapping[Key_I] = 23;
	m_DXKeyMapping[Key_O] = 24;
	m_DXKeyMapping[Key_P] = 25;
	m_DXKeyMapping[Key_LBracket] = 26;
	m_DXKeyMapping[Key_RBracket] = 27;
	m_DXKeyMapping[Key_Return] = 28;
	m_DXKeyMapping[Key_LControl] = 29;
	m_DXKeyMapping[Key_A] = 30;
	m_DXKeyMapping[Key_S] = 31;
	m_DXKeyMapping[Key_D] = 32;
	m_DXKeyMapping[Key_F] = 33;
	m_DXKeyMapping[Key_G] = 34;
	m_DXKeyMapping[Key_H] = 35;
	m_DXKeyMapping[Key_J] = 36;
	m_DXKeyMapping[Key_K] = 37;
	m_DXKeyMapping[Key_L] = 38;
	m_DXKeyMapping[Key_Semicolon] = 39;
	m_DXKeyMapping[Key_Apostrophe] = 40;
	m_DXKeyMapping[Key_Grave] = 41;
	m_DXKeyMapping[Key_LShift] = 42;
	m_DXKeyMapping[Key_Backslash] = 43;
	m_DXKeyMapping[Key_Z] = 44;
	m_DXKeyMapping[Key_X] = 45;
	m_DXKeyMapping[Key_C] = 46;
	m_DXKeyMapping[Key_V] = 47;
	m_DXKeyMapping[Key_B] = 48;
	m_DXKeyMapping[Key_N] = 49;
	m_DXKeyMapping[Key_M] = 50;
	m_DXKeyMapping[Key_Comma] = 51;
	m_DXKeyMapping[Key_Period] = 52;
	m_DXKeyMapping[Key_Slash] = 53;
	m_DXKeyMapping[Key_RShift] = 54;
	m_DXKeyMapping[Key_Multiply] = 55;
	m_DXKeyMapping[Key_LAlt] = 56;
	m_DXKeyMapping[Key_Space] = 57;
	m_DXKeyMapping[Key_Capital] = 58;
	m_DXKeyMapping[Key_F1] = 59;
	m_DXKeyMapping[Key_F2] = 60;
	m_DXKeyMapping[Key_F3] = 61;
	m_DXKeyMapping[Key_F4] = 62;
	m_DXKeyMapping[Key_F5] = 63;
	m_DXKeyMapping[Key_F6] = 64;
	m_DXKeyMapping[Key_F7] = 65;
	m_DXKeyMapping[Key_F8] = 66;
	m_DXKeyMapping[Key_F9] = 67;
	m_DXKeyMapping[Key_F10] = 68;
	m_DXKeyMapping[Key_Numlock] = 69;
	m_DXKeyMapping[Key_Scroll] = 70;
	m_DXKeyMapping[Key_Numpad7] = 71;
	m_DXKeyMapping[Key_Numpad8] = 72;
	m_DXKeyMapping[Key_Numpad9] = 73;
	m_DXKeyMapping[Key_Subtract] = 74;
	m_DXKeyMapping[Key_Numpad4] = 75;
	m_DXKeyMapping[Key_Numpad5] = 76;
	m_DXKeyMapping[Key_Numpad6] = 77;
	m_DXKeyMapping[Key_Add] = 78;
	m_DXKeyMapping[Key_Numpad1] = 79;
	m_DXKeyMapping[Key_Numpad2] = 80;
	m_DXKeyMapping[Key_Numpad3] = 81;
	m_DXKeyMapping[Key_Numpad0] = 82;
	m_DXKeyMapping[Key_Decimal] = 83;
	m_DXKeyMapping[Key_F11] = 87;
	m_DXKeyMapping[Key_F12] = 88;
	m_DXKeyMapping[Key_NumpAdenter] = 156;
	m_DXKeyMapping[Key_RControl] = 157;
	m_DXKeyMapping[Key_Divide] = 181;
	m_DXKeyMapping[Key_Sysrq] = 183;
	m_DXKeyMapping[Key_RAlt] = 184;
	m_DXKeyMapping[Key_Pause] = 197;
	m_DXKeyMapping[Key_Home] = 199;
	m_DXKeyMapping[Key_Up] = 200;
	m_DXKeyMapping[Key_PageUp] = 201;
	m_DXKeyMapping[Key_Left] = 203;
	m_DXKeyMapping[Key_Right] = 205;
	m_DXKeyMapping[Key_End] = 207;
	m_DXKeyMapping[Key_Down] = 208;
	m_DXKeyMapping[Key_PageDown] = 209;
	m_DXKeyMapping[Key_Insert] = 210;
	m_DXKeyMapping[Key_Delete] = 211;
	m_DXKeyMapping[Key_LeftWindows] = 219;
	m_DXKeyMapping[Key_RightWindows] = 220;
	m_DXKeyMapping[Key_Apps] = 221;
}

void RawInputSurveyer::KbmZeroInputs()
{
	memset( &m_MouseState, 0, sizeof( DIMOUSESTATE2 ) );
	memset( m_Keybuffer, 0, sizeof( m_Keybuffer ) );
}

void RawInputSurveyer::KbmShutdown()
{
#if HE_INPUT_USE_XINPUT
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
		m_pKeyboard = nullptr;
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = nullptr;
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
		m_pDirectInput = nullptr;
	}
#endif // HE_INPUT_USE_XINPUT
}

void RawInputSurveyer::KbmUpdate()
{
#if HE_WIN64 || HE_WIN32

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
		if (m_MouseAcquired)
		{
			AcquireMouse();
			m_pMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &m_MouseState );
		}

		AcquireKeyboard();
		m_pKeyboard->GetDeviceState( sizeof( m_Keybuffer ), m_Keybuffer );
	}

#endif // HE_WIN64 || HE_WIN32
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
