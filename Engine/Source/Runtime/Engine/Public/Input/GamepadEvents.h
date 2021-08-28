// Copyright Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"
#include "Input/InputEnums.h"


class GamepadThumbstickMovedEvent : public Event, public InputEvent
{
public:
	GamepadThumbstickMovedEvent( float MoveDelta, DigitalInput KeyMapCode )
		: InputEvent( KeyMapCode, IE_Moved )
		, m_MoveDelta( MoveDelta )
	{
	}

	inline float GetMoveDelta() const { return m_MoveDelta; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadThumbstickMovedEvent: " << m_MoveDelta ;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_GamepadThumbstickMoved )
		EVENT_CLASS_CATEGORY( EC_Gamepad | EC_Input )

private:
	float m_MoveDelta;
};


class GamepadTriggerMovedEvent : public Event, public InputEvent
{
public:
	GamepadTriggerMovedEvent( float MoveDelta, DigitalInput KeyMapCode )
		: InputEvent( KeyMapCode, IE_Moved )
		, m_MoveDelta( MoveDelta )
	{
	}

	inline float GetMoveDelta() const { return m_MoveDelta; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadThumbstickMovedEvent: " << m_MoveDelta;
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_GamepadThumbstickMoved )
		EVENT_CLASS_CATEGORY( EC_Gamepad | EC_Input )

private:
	float m_MoveDelta;
};