// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


enum EInputEvent
{
	IE_Pressed	= 0,
	IE_Released = 1,
	IE_Held		= 2,
	IE_Typed	= 3,
	IE_Moved	= 4,
};

enum EGampadRumbleMotor
{
	GRM_Left	= 0x00,
	GRM_Right	= 0x01,
};

enum DigitalInput
{
	// CAUTION: Editing these has repercussions on input mapping files such as "InputMappings.ini"
	// Keyboard
	// Key_ must start at zero, see m_DXKeyMapping
	Key_Escape = 0,
	Key_1,
	Key_2,
	Key_3,
	Key_4,
	Key_5,
	Key_6,
	Key_7,
	Key_8,
	Key_9,
	Key_0,
	Key_Minus,
	Key_Equals,
	Key_Back,
	Key_Tab,
	Key_Q,
	Key_W,
	Key_E,
	Key_R,
	Key_T,
	Key_Y,
	Key_U,
	Key_I,
	Key_O,
	Key_P,
	Key_LBracket,
	Key_RBracket,
	Key_Return,
	Key_LControl,
	Key_A,
	Key_S,
	Key_D,
	Key_F,
	Key_G,
	Key_H,
	Key_J,
	Key_K,
	Key_L,
	Key_Semicolon,
	Key_Apostrophe,
	Key_Grave,
	Key_LShift,
	Key_Backslash,
	Key_Z,
	Key_X,
	Key_C,
	Key_V,
	Key_B,
	Key_N,
	Key_M,
	Key_Comma,
	Key_Period,
	Key_Slash,
	Key_RShift,
	Key_Multiply,
	Key_LAlt,
	Key_Space,
	Key_Capital,
	Key_F1,
	Key_F2,
	Key_F3,
	Key_F4,
	Key_F5,
	Key_F6,
	Key_F7,
	Key_F8,
	Key_F9,
	Key_F10,
	Key_Numlock,
	Key_Scroll,
	Key_Numpad7,
	Key_Numpad8,
	Key_Numpad9,
	Key_Subtract,
	Key_Numpad4,
	Key_Numpad5,
	Key_Numpad6,
	Key_Add,
	Key_Numpad1,
	Key_Numpad2,
	Key_Numpad3,
	Key_Numpad0,
	Key_Decimal,
	Key_F11,
	Key_F12,
	Key_NumEnter,
	Key_RControl,
	Key_Divide,
	Key_Sysrq,
	Key_RAlt,
	Key_Pause,
	Key_Plus,
	Key_Home,
	Key_Up,
	Key_PageUp,
	Key_Left,
	Key_Right,
	Key_End,
	Key_Down,
	Key_PageDown,
	Key_Insert,
	Key_Delete,
	Key_LeftWindows,
	Key_RightWindows,
	Key_Apps,

	// Gamepad
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	StartButton,
	BackButton,
	LThumbClick,
	RThumbClick,
	LShoulder,
	RShoulder,
	// Xbox
	AButton,
	BButton,
	XButton,
	YButton = 118,
	// Playstation (analogous to Xbox buttons).
	CrossButton		= AButton,
	CircleButton	= BButton,
	SquareButton	= XButton,
	TriangleButton	= YButton,

	// Mouse
	Mouse0,
	Mouse_Left = Mouse0,
	Mouse1,
	Mouse_Right = Mouse1,
	Mouse2,
	Mouse_Middle = Mouse2,
	Mouse3,
	Mouse4,
	Mouse5,
	Mouse6,
	Mouse7 = 126,


	// Gamepad
	
	AnalogLeftTrigger,
	AnalogRightTrigger,
	AnalogLeftStickX,
	AnalogLeftStickY,
	AnalogRightStickX,
	AnalogRightStickY = 132,

	// Mouse
	AnalogMouseX,
	AnalogMouseY,
	AnalogMouseScroll = 136,

	// Misc - Internal
	PlatformDefined,
	MouseAnyMoved,
	MousePositionMoved,
	MouseRawInput
};

static const uint32 kNumKeys			= (Key_Apps - Key_Escape) + 1;
static const uint32 kNumMouseInputs		= (Mouse7 - Mouse0) + 1;
static const uint32 kNumAnalogInputs	= (AnalogMouseScroll - AnalogLeftTrigger) + 1;
static const uint32 kNumDigitalInputs	= (Mouse7 - Key_Escape) + 1;


/*
	True if the value is a valid keyboard key. False if not.
*/
#define IsValidKeyboardKey( Value )		( Value >= Key_Escape ) && ( Value <= Key_Apps )

/*
	True if the value is a valid gamepad button. False if not.
*/
#define IsValidGamepadButton( Value )	( Value >= DPadUp ) && ( Value <= YButton )

/*
	True if the Value is a valid mouse button. False if not.
*/
#define IsValidMouseButton( Value )		( Value >= Mouse0 ) && ( Value <= Mouse7 )

/*
	True if the value is a valid analog input type. False if not.
*/
#define IsValidAnalogInput( Value )		( Value >= AnalogLeftTrigger ) && ( Value <= AnalogMouseScroll )

/*
	True if the input value is in a moving state. False if not.
*/
#define IsAnalogInputMoved( Value )		( Value != 0.f )

/*
	True if the value is a valid mouse analog movement type. False if not.
*/
#define IsMouseAnalog( Value )			( Value >= AnalogMouseX || Value <= AnalogMouseScroll )

/*
	True if the value is a valid mouse module movement type. As in, the movement typed describes the mouve moving up, down, left, or right. False if not.
*/
#define IsMouseMoveAnalog( Value )		( Value >= AnalogMouseX || Value <= AnalogMouseY )

/*
	True if the value is a valid gamepad movement type. As in, the movement typed describes the gamepad thumbsticks or triggers moving up, down, left, or right. False if not.
*/
#define IsGamepadMoveAnalog( Value )	( Value >= AnalogLeftTrigger || Value <= AnalogRightStickY )

/*
	True if the value is a valid gamepad thumbstick movement type. As in, the movement typed describes the gamepad thumbsticks moving up, down, left, or right. False if not.
*/
#define IsValidGamepadThumbStickAnalog( Value )		( Value >= AnalogLeftStickX || Value <= AnalogRightStickY )

/*
	True if the value is a valid gamepad trigger movement type. False if not.
*/
#define IsValidGamepadTriggerAnalog( Value )		( Value == AnalogLeftTrigger || Value == AnalogRightTrigger )

/*
	Internal enums to track state of buttons from frame to frame.
*/
enum
{
	// Current state of buttons.
	kState_Current = 0,
	// Previous state of buttons.
	kState_Previous = 1,

	// Num states.
	kState_Count = 2,
};