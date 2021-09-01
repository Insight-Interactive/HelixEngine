// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"
#include "Input/InputEnums.h"


class MousePositionMovedEvent : public Event, public InputEvent
{
public:
	MousePositionMovedEvent( float x, float y )
		: InputEvent( MouseAnyMoved, IE_Moved )
		, m_MouseX( x )
		, m_MouseY( y ) 
	{
	}

	inline float GetX() const { return m_MouseX; }
	inline float GetY() const { return m_MouseY; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "MousePositionMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_MouseMoved )
	EVENT_CLASS_CATEGORY( EC_Mouse | EC_Input )
	
private:
	float m_MouseX;
	float m_MouseY;
};


class MouseMovedEvent : public Event, public InputEvent
{
public:
	MouseMovedEvent( float Value, DigitalInput KeyMapCode )
		: InputEvent( KeyMapCode, IE_Moved )
		, m_Value( Value )
	{
	}

	inline float GetDelta() const { return m_Value; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_Value;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_MouseMoved )
		EVENT_CLASS_CATEGORY( EC_Mouse | EC_Input )

private:
	float m_Value;
};


class MouseWheelScrolledEvent : public Event, public InputEvent
{
public:
	MouseWheelScrolledEvent( float xOffset, float yOffset )
		: InputEvent( AnalogMouseScroll, IE_Moved )
		, m_XOffset( xOffset )
		, m_YOffset( yOffset ) 
	{
	}

	inline float GetXOffset() const { return m_XOffset; }
	inline float GetYOffset() const { return m_YOffset; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_MouseScrolled )
	EVENT_CLASS_CATEGORY( EC_Mouse | EC_Input )

private:
	float m_XOffset;
	float m_YOffset;
};


class MouseButtonEvent : public Event, public InputEvent
{
public:

	EVENT_CLASS_CATEGORY( EC_Mouse | EC_Input )

protected:
	MouseButtonEvent( DigitalInput KeyMapCode, EInputEvent EventType )
		: InputEvent( KeyMapCode, EventType ) 
	{
	}

};


class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent( DigitalInput KeyMapCode )
		: MouseButtonEvent( KeyMapCode, IE_Pressed ) 
	{
	}

	EVENT_CLASS_TYPE( ET_MouseButtonPressed )
};


class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent( DigitalInput KeyMapCode )
		: MouseButtonEvent( KeyMapCode, IE_Released ) 
	{
	}

	EVENT_CLASS_TYPE( ET_MouseButtonReleased )
};
