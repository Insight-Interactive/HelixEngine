// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/InputEnums.h"


class FKeyboard
{
	friend class FRawInputSurveyer;
public:
	FKeyboard();
	~FKeyboard();

	bool IsValid() const;

	void Acquire();
	void Unacquire();
	void QueryState();
	void ZeroInput();

	bool GetIsAcquired() const;
	bool GetKey( uint32 KeyIndex ) const;

private:
	void BuildKeyBuffer();
	void SetKey( const uint8& Key, const bool& IsPressed );
	void SetIsAcquired( const bool& Value );
	void SetupPlatformHIDProvider( void* pProviderInterface, void* pNativeWindow );
	void Destroy();

private:
	bool m_IsAcquired;
	uint8 m_KeyBuffer[256];
	uint8 m_KeyMappings[kNumKeys];

};


//
// Inline function implementations
//

inline void FKeyboard::Destroy()
{
	if (IsValid())
	{
		Unacquire();
	}
}

inline bool FKeyboard::IsValid() const
{
	return true;
}

inline void FKeyboard::Acquire()
{
	SetIsAcquired( true );
}

inline void FKeyboard::Unacquire()
{
	SetIsAcquired( false );
}

inline void FKeyboard::ZeroInput()
{
	ZeroMemory( &m_KeyBuffer, sizeof( m_KeyBuffer ) );
}

inline bool FKeyboard::GetIsAcquired() const
{
	return m_IsAcquired;
}

inline bool FKeyboard::GetKey( uint32 Key ) const
{
	return m_KeyBuffer[m_KeyMappings[Key]];
}

inline void FKeyboard::QueryState()
{
}

inline void FKeyboard::SetIsAcquired( const bool& Value )
{
	m_IsAcquired = Value;
}

inline void FKeyboard::SetKey( const uint8& Key, const bool& IsPressed )
{
	m_KeyBuffer[Key] = IsPressed;
}

inline void FKeyboard::BuildKeyBuffer()
{
	// Map DigitalInput enum to standard chars.

	// Numbers
	m_KeyMappings[Key_0] = '0';
	m_KeyMappings[Key_1] = '1';
	m_KeyMappings[Key_2] = '2';
	m_KeyMappings[Key_3] = '3';
	m_KeyMappings[Key_4] = '4';
	m_KeyMappings[Key_5] = '5';
	m_KeyMappings[Key_6] = '6';
	m_KeyMappings[Key_7] = '7';
	m_KeyMappings[Key_8] = '8';
	m_KeyMappings[Key_9] = '9';

	// Letters
	m_KeyMappings[Key_A] = 'A';
	m_KeyMappings[Key_B] = 'B';
	m_KeyMappings[Key_C] = 'C';
	m_KeyMappings[Key_D] = 'D';
	m_KeyMappings[Key_E] = 'E';
	m_KeyMappings[Key_F] = 'F';
	m_KeyMappings[Key_G] = 'G';
	m_KeyMappings[Key_H] = 'H';
	m_KeyMappings[Key_I] = 'I';
	m_KeyMappings[Key_J] = 'J';
	m_KeyMappings[Key_K] = 'K';
	m_KeyMappings[Key_L] = 'L';
	m_KeyMappings[Key_M] = 'M';
	m_KeyMappings[Key_N] = 'N';
	m_KeyMappings[Key_O] = 'O';
	m_KeyMappings[Key_P] = 'P';
	m_KeyMappings[Key_Q] = 'Q';
	m_KeyMappings[Key_R] = 'R';
	m_KeyMappings[Key_S] = 'S';
	m_KeyMappings[Key_T] = 'T';
	m_KeyMappings[Key_U] = 'U';
	m_KeyMappings[Key_V] = 'V';
	m_KeyMappings[Key_W] = 'W';
	m_KeyMappings[Key_X] = 'X';
	m_KeyMappings[Key_Y] = 'Y';
	m_KeyMappings[Key_Z] = 'Z';

	// Platform
#if HE_WINDOWS
	m_KeyMappings[Key_Grave]		= VK_OEM_3;
	m_KeyMappings[Key_Period]		= VK_OEM_PERIOD;
	m_KeyMappings[Key_Slash]		= VK_OEM_2;
	m_KeyMappings[Key_Decimal]		= VK_DECIMAL;
	m_KeyMappings[Key_Comma]		= VK_OEM_COMMA;
	m_KeyMappings[Key_Equals]		= VK_OEM_NEC_EQUAL;
	m_KeyMappings[Key_Add]			= VK_OEM_PLUS;
	m_KeyMappings[Key_Subtract]		= VK_SUBTRACT;
	m_KeyMappings[Key_Multiply]		= VK_MULTIPLY;
	m_KeyMappings[Key_Divide]		= VK_DIVIDE;
	m_KeyMappings[Key_Plus]			= VK_OEM_PLUS;
	m_KeyMappings[Key_Minus]		= VK_OEM_MINUS;
	m_KeyMappings[Key_Semicolon]	= VK_OEM_1;
	m_KeyMappings[Key_Escape]		= VK_ESCAPE;
	m_KeyMappings[Key_Back]			= VK_BACK;
	m_KeyMappings[Key_Space]		= VK_SPACE;
	m_KeyMappings[Key_Tab]			= VK_TAB;
	m_KeyMappings[Key_Return]		= VK_RETURN;
	m_KeyMappings[Key_LControl]		= VK_LCONTROL;
	m_KeyMappings[Key_RControl]		= VK_RCONTROL;
	m_KeyMappings[Key_LShift]		= VK_LSHIFT;
	m_KeyMappings[Key_RShift]		= VK_RSHIFT;
	m_KeyMappings[Key_LAlt]			= VK_LMENU;
	m_KeyMappings[Key_RAlt]			= VK_RMENU;
	m_KeyMappings[Key_Capital]		= VK_CAPITAL;
	m_KeyMappings[Key_F1]			= VK_F1;
	m_KeyMappings[Key_F2]			= VK_F2;
	m_KeyMappings[Key_F3]			= VK_F3;
	m_KeyMappings[Key_F4]			= VK_F4;
	m_KeyMappings[Key_F5]			= VK_F5;
	m_KeyMappings[Key_F6]			= VK_F6;
	m_KeyMappings[Key_F7]			= VK_F7;
	m_KeyMappings[Key_F8]			= VK_F8;
	m_KeyMappings[Key_F9]			= VK_F9;
	m_KeyMappings[Key_F10]			= VK_F10;
	m_KeyMappings[Key_F11]			= VK_F11;
	m_KeyMappings[Key_F12]			= VK_F12;
	m_KeyMappings[Key_Scroll]		= VK_SCROLL;
	m_KeyMappings[Key_Numlock]		= VK_NUMLOCK;
	m_KeyMappings[Key_NumEnter]		= VK_RETURN;
	m_KeyMappings[Key_Numpad0]		= VK_NUMPAD0;
	m_KeyMappings[Key_Numpad1]		= VK_NUMPAD1;
	m_KeyMappings[Key_Numpad2]		= VK_NUMPAD2;
	m_KeyMappings[Key_Numpad3]		= VK_NUMPAD3;
	m_KeyMappings[Key_Numpad4]		= VK_NUMPAD4;
	m_KeyMappings[Key_Numpad5]		= VK_NUMPAD5;
	m_KeyMappings[Key_Numpad6]		= VK_NUMPAD6;
	m_KeyMappings[Key_Numpad7]		= VK_NUMPAD7;
	m_KeyMappings[Key_Numpad8]		= VK_NUMPAD8;
	m_KeyMappings[Key_Numpad9]		= VK_NUMPAD9;
	m_KeyMappings[Key_Sysrq]		= VK_SNAPSHOT;
	m_KeyMappings[Key_Pause]		= VK_PAUSE;
	m_KeyMappings[Key_Home]			= VK_HOME;
	m_KeyMappings[Key_Up]			= VK_UP;
	m_KeyMappings[Key_Down]			= VK_DOWN;
	m_KeyMappings[Key_Left]			= VK_LEFT;
	m_KeyMappings[Key_Right]		= VK_RIGHT;
	m_KeyMappings[Key_PageUp]		= VK_PRIOR;
	m_KeyMappings[Key_PageDown]		= VK_NEXT;
	m_KeyMappings[Key_End]			= VK_END;
	m_KeyMappings[Key_Insert]		= VK_INSERT;
	m_KeyMappings[Key_Delete]		= VK_DELETE;
	m_KeyMappings[Key_LeftWindows]	= VK_LWIN;
	m_KeyMappings[Key_RightWindows]	= VK_RWIN;
	m_KeyMappings[Key_Apps]			= VK_APPS;
	// US keyboard
	m_KeyMappings[Key_LBracket]		= VK_OEM_4;
	m_KeyMappings[Key_RBracket]		= VK_OEM_6;
	m_KeyMappings[Key_Apostrophe]	= VK_OEM_7;
	m_KeyMappings[Key_Backslash]	= VK_OEM_5;

#endif
}
