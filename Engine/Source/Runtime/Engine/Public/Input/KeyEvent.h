// Copyright Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"
#include "Input/InputEnums.h"


class KeyEvent : public Event, public InputEvent
{
public:

	EVENT_CLASS_CATEGORY( EC_Keyboard | EC_Input )
	
protected:
	KeyEvent( DigitalInput Keycode, EInputEvent Status )
		: InputEvent( Keycode, Status ) 
	{
	}

};


class KeyPressedEvent : public KeyEvent
{
public:
	explicit KeyPressedEvent( DigitalInput KeyCode, int32 RepeatCount )
		: KeyEvent( KeyCode, IE_Pressed )
		, m_RepeatCount( RepeatCount )
	{
	}
	/*
		An ambigious event for the platform key messages. The actual window 
		does not give DigitalInput enumerations, this will allow the storage
		of the platform keystroke.
	*/
	explicit KeyPressedEvent( int32 PlatformKeycode, int32 RepeatCount )
		: KeyEvent( PlatformDefined, IE_Pressed )
		, m_RepeatCount( RepeatCount )
		, m_PlatformKeycode( PlatformKeycode )
	{
	}

	inline int32 GetPlatformKeycode()	const { return m_PlatformKeycode; }
	inline int32 GetRepeatCount()		const { return m_RepeatCount; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyMapCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_KeyPressed )
	
private:

private:
	int32	m_RepeatCount;
	int32	m_PlatformKeycode;
};

class KeyHeldEvent : public KeyEvent
{
public:
	KeyHeldEvent( DigitalInput Keycode, float HoldTime )
		: KeyEvent( Keycode, IE_Held )
		, m_HoldTime( HoldTime )
	{
	}

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyHeldEvent: " << m_KeyMapCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_KeyHeld )

private:
	float m_HoldTime;

};

class KeyReleasedEvent : public KeyEvent
{
public:
	explicit KeyReleasedEvent( DigitalInput Keycode )
		: KeyEvent( Keycode, IE_Released ) 
		, m_PlatformKeycode( -1 )
	{
	}
	/*
		An ambigious event for the platform key messages. The actual window
		does not give DigitalInput enumerations, this will allow the storage
		of the platform keystroke without having to to a complete/expensive conversion.
	*/
	explicit KeyReleasedEvent( int32 PlatformKeycode )
		: KeyEvent( PlatformDefined, IE_Pressed )
		, m_PlatformKeycode( PlatformKeycode )
	{
	}

	inline int32 GetPlatformKeycode()	const { return m_PlatformKeycode; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyMapCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_KeyReleased )
private:
	int32	m_PlatformKeycode;

};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent( DigitalInput Keycode )
		: KeyEvent( Keycode, IE_Typed ) 
	{
	}

	String ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyMapCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_KeyTyped )

};
