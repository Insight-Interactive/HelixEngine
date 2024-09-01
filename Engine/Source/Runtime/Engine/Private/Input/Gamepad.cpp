// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Input/Gamepad.h"


// FGamepad
//

FGamepad::FGamepad()
{
	Deactivate();
}

FGamepad::~FGamepad()
{
}

void FGamepad::Tick( float DeltaTime )
{
	CopyMemory( m_Buttons[kState_Previous], m_Buttons[kState_Current], sizeof( m_Buttons[kState_Current] ) );
	ZeroMemory( m_Buttons[kState_Current], sizeof( m_Buttons[kState_Current] ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );

#if HE_INPUT_USE_XINPUT
	ZeroMemory( &m_State, sizeof( XINPUT_STATE ) );
	DWORD dwResult = XInputGetState( m_OwnerIndex, &m_State );

	// Update the state.
	if (dwResult == ERROR_SUCCESS)
	{
		// Update Buttons
		//
		if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			m_Buttons[kState_Current][kButton_A] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			m_Buttons[kState_Current][kButton_B] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			m_Buttons[kState_Current][kButton_X] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			m_Buttons[kState_Current][kButton_Y] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			m_Buttons[kState_Current][kButton_DPadUp] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_Buttons[kState_Current][kButton_DPadDown] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			m_Buttons[kState_Current][kButton_DPadLeft] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			m_Buttons[kState_Current][kButton_DPadRight] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			m_Buttons[kState_Current][kButton_Start] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			m_Buttons[kState_Current][kButton_Select] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			m_Buttons[kState_Current][kButton_LeftShoulder] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			m_Buttons[kState_Current][kButton_RightShoulder] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			m_Buttons[kState_Current][kButton_LeftThumb] = true;
		}
		else if (m_State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			m_Buttons[kState_Current][kButton_RightThumb] = true;
		}
		

		// Update Analog Values
		//
		const float kTriggerMaxValue = 255.f;
		m_Analogs[kAnalog_LeftTrigger] = m_State.Gamepad.bLeftTrigger / kTriggerMaxValue;
		m_Analogs[kAnalog_RightTrigger] = m_State.Gamepad.bRightTrigger / kTriggerMaxValue;
		m_Analogs[kAnalog_LeftStickX] = FilterAnalogInput( m_State.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		m_Analogs[kAnalog_LeftStickY] = FilterAnalogInput( m_State.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
		m_Analogs[kAnalog_RightStickX] = FilterAnalogInput( m_State.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
		m_Analogs[kAnalog_RightStickY] = FilterAnalogInput( m_State.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
	}
	// Controller is disconnected.
	else
	{
		SetConnectionState( GCS_Disconnected );
	}
#endif
}

float FGamepad::GetBatteryPercentage()
{
	float BatteryLevelNormalized = 0.f;
#if HE_INPUT_USE_XINPUT
	XINPUT_BATTERY_INFORMATION BatteryInfo;
	DWORD dwResult = XInputGetBatteryInformation( m_OwnerIndex, BATTERY_DEVTYPE_GAMEPAD, &BatteryInfo );
	if (dwResult == ERROR_SUCCESS)
	{
		switch (BatteryInfo.BatteryType)
		{
		case BATTERY_TYPE_DISCONNECTED:
			SetConnectionState( GCS_Disconnected );
			break;
		case BATTERY_TYPE_WIRED:
			BatteryLevelNormalized = 1.f;
			break;
		default:
			BatteryLevelNormalized = 1.f;
			break;
		}
		switch (BatteryInfo.BatteryLevel)
		{
		case BATTERY_LEVEL_EMPTY:
			BatteryLevelNormalized = 0.f;
			break;
		case BATTERY_LEVEL_LOW:
			BatteryLevelNormalized = 0.25f;
			break;
		case BATTERY_LEVEL_MEDIUM:
			BatteryLevelNormalized = 0.5f;
			break;
		case BATTERY_LEVEL_FULL:
			BatteryLevelNormalized = 1.f;
			break;
		default:
			BatteryLevelNormalized = 0.f;
			break;
		}
	}
#endif
	return BatteryLevelNormalized;
}

float FGamepad::FilterAnalogInput( int Val, int DeadZone )
{
#if HE_INPUT_USE_XINPUT
	if (Val < 0)
	{
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


// FGamepadQueryBatcher
//

FGamepadQueryBatcher::FGamepadQueryBatcher()
	: m_PollRate( 0.01f )
	, m_PollRateTimer( 0.f )
{

}

FGamepadQueryBatcher::~FGamepadQueryBatcher()
{
	for (uint32 i = 0; i < kMaxSimultaneousGamepads; ++i)
	{
		m_Gamepads[i].Deactivate();
	}
}

FGamepad* FGamepadQueryBatcher::CreateGamepad()
{
	for (uint32 i = 0; i < kMaxSimultaneousGamepads; ++i)
	{
		if (!m_Gamepads[i].IsValid())
		{
			m_Gamepads[i].Activate( (GamepadHandle)i );
			return &m_Gamepads[i];
		}
	}
	return NULL;
}

void FGamepadQueryBatcher::DestroyGamepad( GamepadHandle Handle )
{
	HE_ASSERT( Handle >= 0 && Handle <= kMaxSimultaneousGamepads );

	m_Gamepads[Handle].Deactivate();
}

void FGamepadQueryBatcher::Tick( float DeltaTime )
{
	// Throttle the polling to increase performce.
	// We don't want to make a round trip from the 
	// connection point to the controller every frame.
	m_PollRateTimer += DeltaTime;
	if (m_PollRateTimer > m_PollRate)
	{
		m_PollRateTimer = 0.f;

		for (uint32 i = 0; i < kMaxSimultaneousGamepads; ++i)
		{
			if (m_Gamepads[i].IsActivated())
			{
				m_Gamepads[i].Tick( DeltaTime );
			}
		}
	}
}
