// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/InputEnums.h"


class FKeyboard
{
	friend class RawInputSurveyer;
public:
	FKeyboard();
	~FKeyboard();

	bool IsValid() const;

	void Acquire();
	void Unacquire();
	void QueryState();
	void ZeroInput();

	bool GetState( uint32 KeyIndex ) const;

private:
	void BuildKeyBuffer();
	void SetIsAcquired( const bool& Value );
	void SetupPlatformRIDProvider( void* pProviderInterface, void* pNativeWindow );
	void Destroy();

private:
	bool m_IsAcquired;

#if HE_INPUT_USE_XINPUT
	IDirectInputDevice8A* m_pDIDKeyboard;
	uint8 m_Keybuffer[256];
	uint8 m_DXKeyMapping[NumKeys]; // Map DigitalInput enum to DX key codes 
#endif

};


//
// Inline function implementations
//

inline void FKeyboard::Destroy()
{
	if (IsValid())
	{
#if HE_INPUT_USE_XINPUT
		m_pDIDKeyboard->Unacquire();
		m_pDIDKeyboard->Release();
		m_pDIDKeyboard = NULL;
#endif
	}
}

inline bool FKeyboard::IsValid() const
{
#if HE_INPUT_USE_XINPUT
	return m_pDIDKeyboard != NULL;
#else
	return false;
#endif
}

inline void FKeyboard::Acquire()
{
	SetIsAcquired( true );

#if HE_INPUT_USE_XINPUT
	m_pDIDKeyboard->Acquire();
#endif
}

inline void FKeyboard::Unacquire()
{
	SetIsAcquired( false );

#if HE_INPUT_USE_XINPUT
	m_pDIDKeyboard->Unacquire();
#endif
}

inline void FKeyboard::ZeroInput()
{
#if HE_INPUT_USE_XINPUT
	ZeroMemory( &m_Keybuffer, sizeof( m_Keybuffer ) );
#endif
}

inline bool FKeyboard::GetState( uint32 KeyIndex ) const
{
#if HE_INPUT_USE_XINPUT
	return (m_Keybuffer[m_DXKeyMapping[KeyIndex]] & 0x80);
#else
	return false;
#endif
}

inline void FKeyboard::QueryState()
{
#if HE_INPUT_USE_XINPUT
	m_pDIDKeyboard->GetDeviceState( sizeof( m_Keybuffer ), m_Keybuffer );
#endif
}

inline void FKeyboard::SetIsAcquired( const bool& Value )
{
	m_IsAcquired = Value;
}

inline void FKeyboard::BuildKeyBuffer()
{
#if HE_INPUT_USE_XINPUT
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
#endif
}
