#pragma once

#include "Input/InputEnums.h"


static const uint8 kMaxSimultaneousGamepads =
#if HE_INPUT_USE_XINPUT
XUSER_MAX_COUNT;
#else
4;
#endif


typedef uint8 GamepadHandle;
#define HE_INVALID_GAMEPAD_HANDLE (GamepadHandle)(-1)

/*
	Encapsulated a gampad controller.
*/
class FGamepad
{
	friend class FGamepadQueryBatcher;
	friend class FRawInputSurveyer;
public:
	enum EGamepadConnectionState
	{
		GCS_Connected,
		GCS_Disconnected,
	};
public:
	FGamepad();
	~FGamepad();

	/*
		Retuns true if the gamepad is in a valid state (can send and receive information). False if not.
	*/
	bool IsValid() const;
	
	/*
		Returns the user index of this gamepad.
	*/
	const GamepadHandle GetHandle() const;

	/*
		Returns true if the gamepad is connected and receiving information. False if not.
	*/
	bool IsConnected() const;
	
	/*
		Returns a normlized number representing the battery level of the gamepad. 
		0 being completly empty, 1 being fully-charged.
	*/
	float GetBatteryPercentage();

	/*
		Returns the connection state of the gamepad.
	*/
	EGamepadConnectionState GetConnectionState() const;

	/*
		Returns true if the gamepad is activated and able to send/receive information.
	*/
	bool IsActivated() const;

	bool IsButtonPressed( uint32 Button )
	{
		return m_Buttons[kState_Current][Button];
	}

	float GetAnalogValue( uint32 AnalogValue )
	{
		return m_Analogs[AnalogValue];
	}

protected:
	/*
		Updates the gamepad state by quirying the device itself. This should 
		be called conservitivly to save performance.
	*/
	void Tick( float DeltaTime );

	/*
		Set the user index of this gamepad.
	*/
	void SetHandle( GamepadHandle Index );
	
	/*
		Sets the connection state of this gamepad.
	*/
	void SetConnectionState( EGamepadConnectionState State );

	/*
		Establish a connection with the gamepad.
		@param NewHandle - The user index of the gamepad.
	*/
	void Activate( GamepadHandle NewHandle );

	/*
		Terminate the connection with the gamepad.
	*/
	void Deactivate();

	float FilterAnalogInput( int Val, int DeadZone );


protected:
	bool m_IsActivated;
	GamepadHandle m_OwnerIndex;
	EGamepadConnectionState m_ConnectionState;

	enum
	{
		kAnalog_LeftTrigger,
		kAnalog_RightTrigger,
		kAnalog_LeftStickX,
		kAnalog_LeftStickY,
		kAnalog_RightStickX,
		kAnalog_RightStickY,
		kAnalog_TouchpadX,
		kAnalog_TouchpadY,

		kAnalog_Count,
	};
	float m_Analogs[kAnalog_Count];

	enum
	{
		kButton_A, // Playstation - Cross
		kButton_B, // Playstation - Circle
		kButton_X, // Playstation - Square
		kButton_Y, // Playstation - Triangle
		kButton_DPadUp,
		kButton_DPadDown,
		kButton_DPadLeft,
		kButton_DPadRight,
		kButton_Start,
		kButton_Select,
		kButton_LeftThumb,
		kButton_RightThumb,
		kButton_LeftShoulder,
		kButton_RightShoulder,
		kButton_TouchPad,

		kButton_Count,
	};
	bool m_Buttons[kState_Count][kButton_Count];

#if HE_INPUT_USE_XINPUT
	XINPUT_STATE m_State;
#endif

};


class FGamepadQueryBatcher
{
public:
	FGamepadQueryBatcher();
	~FGamepadQueryBatcher();

	FGamepad* CreateGamepad();
	void DestroyGamepad( GamepadHandle Handle );

	void Tick( float DeltaTime );

private:
	FGamepad m_Gamepads[kMaxSimultaneousGamepads];
	float m_PollRate;
	float m_PollRateTimer;
};


//
// Inline function implementations
//

// FGamepad
//

FORCEINLINE bool FGamepad::IsValid() const
{
	return IsActivated();
}

FORCEINLINE bool FGamepad::IsActivated() const
{
	return m_IsActivated;
}

FORCEINLINE void FGamepad::Activate(GamepadHandle NewHandle)
{
	HE_ASSERT( NewHandle != HE_INVALID_GAMEPAD_HANDLE );

	m_IsActivated = true;
	SetHandle( NewHandle );
}

FORCEINLINE void FGamepad::Deactivate()
{
	m_IsActivated = false;
	SetConnectionState( GCS_Disconnected );
	SetHandle( HE_INVALID_GAMEPAD_HANDLE );
	ZeroMemory( m_Buttons[kState_Current], sizeof( m_Buttons[kState_Current] ) );
	ZeroMemory( m_Analogs, sizeof( m_Analogs ) );
}

FORCEINLINE FGamepad::EGamepadConnectionState FGamepad::GetConnectionState() const
{
	return m_ConnectionState;
}

FORCEINLINE void FGamepad::SetConnectionState(EGamepadConnectionState State)
{
	m_ConnectionState = State;
}

FORCEINLINE void FGamepad::SetHandle( GamepadHandle Index )
{
	m_OwnerIndex = Index;
}

FORCEINLINE const GamepadHandle FGamepad::GetHandle() const
{
	return m_OwnerIndex;
}

FORCEINLINE bool FGamepad::IsConnected() const
{
	return GetConnectionState() == GCS_Disconnected;
}
