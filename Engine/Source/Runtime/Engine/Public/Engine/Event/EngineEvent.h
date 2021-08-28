// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"


// -----------------
//	Window Events
// -----------------

class WindowClosedEvent : public Event
{
public:
	EVENT_CLASS_TYPE( ET_WindowClose )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(uint32 Width, uint32 Height)
		: m_Width(Width)
		, m_Height(Height)
	{
	}
	~WindowResizeEvent()
	{
	}

	inline uint32 GetWidth()	const { return m_Width; }
	inline uint32 GetHeight()	const { return m_Height; }

	String ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE( ET_WindowResize )
		EVENT_CLASS_CATEGORY( EC_Window )

private:
	uint32 m_Width;
	uint32 m_Height;

};


class WindowFocusEvent : public Event
{
public:
	EVENT_CLASS_TYPE( ET_WindowFocus )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowLostFocusEvent : public Event
{
public:
	EVENT_CLASS_TYPE( ET_WindowLostFocus )
		EVENT_CLASS_CATEGORY( EC_Window )
};