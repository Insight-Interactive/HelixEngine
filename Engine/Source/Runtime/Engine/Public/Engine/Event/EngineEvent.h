// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Event/Event.h"


// -----------------
//	FWindow Events
// -----------------

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent()
	{
	}
	virtual ~WindowClosedEvent()
	{
	}

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
	virtual ~WindowResizeEvent()
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


class WindowMaximizedEvent : public Event
{
public:
	WindowMaximizedEvent()
	{
	}
	virtual ~WindowMaximizedEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_WindowMaximized )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowMinimizedEvent : public Event
{
public:
	WindowMinimizedEvent()
	{
	}
	virtual ~WindowMinimizedEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_WindowMinimized )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent()
	{
	}
	virtual ~WindowFocusEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_WindowFocus )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowLostFocusEvent : public Event
{
public:
	WindowLostFocusEvent()
	{
	}
	virtual ~WindowLostFocusEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_WindowLostFocus )
		EVENT_CLASS_CATEGORY( EC_Window )
};


class WindowFileDropEvent : public Event
{
public:
	WindowFileDropEvent( const WChar* FilePath )
		: m_FilePath( FilePath )
	{
	}
	virtual ~WindowFileDropEvent()
	{
	}

	const WString& GetFileName() const { return m_FilePath; }
	

	EVENT_CLASS_TYPE( ET_WindowFileDrop )
		EVENT_CLASS_CATEGORY( EC_Window )

protected:
	WString m_FilePath;
};


// -----------------
//	Engine Events
// -----------------

class EngineEvent : public Event
{
public:
	EngineEvent()
	{
	}
	virtual ~EngineEvent()
	{
	}

};

class EnginePreStartupEvent : public EngineEvent
{
public:
	EnginePreStartupEvent() {}
	virtual ~EnginePreStartupEvent() {}

	EVENT_CLASS_TYPE( ET_EnginePreStartup )
		EVENT_CLASS_CATEGORY( EC_Engine )
};

class EngineStartupEvent : public EngineEvent
{
public:
	EngineStartupEvent() {}
	virtual ~EngineStartupEvent() {}

	EVENT_CLASS_TYPE( ET_EngineStartup )
		EVENT_CLASS_CATEGORY( EC_Engine )
};

class EnginePostStartupEvent : public EngineEvent
{
public:
	EnginePostStartupEvent() {}
	virtual ~EnginePostStartupEvent() {}

	EVENT_CLASS_TYPE( ET_EnginePostStartup )
		EVENT_CLASS_CATEGORY( EC_Engine )
};

class EngineBeginPlayEvent : public EngineEvent
{
public:
	EngineBeginPlayEvent()
	{
	}
	virtual ~EngineBeginPlayEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_EngineBeginPlay )
		EVENT_CLASS_CATEGORY( EC_Application )
};

class EngineEndPlayEvent : public EngineEvent
{
public:
	EngineEndPlayEvent()
	{
	}
	virtual ~EngineEndPlayEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_EngineEndPlay )
		EVENT_CLASS_CATEGORY( EC_Application )
};

class EngineTickEvent : public EngineEvent
{
public:
	EngineTickEvent( float DeltaTime )
		: m_DeltaTime (DeltaTime)
	{
	}
	virtual ~EngineTickEvent()
	{
	}

	inline float GetDeltaTime() { return m_DeltaTime; }

	EVENT_CLASS_TYPE( ET_EngineTick )
		EVENT_CLASS_CATEGORY( EC_Application )

private:
	float m_DeltaTime;

};

class EngineRenderEvent : public EngineEvent
{
public:
	EngineRenderEvent()
	{
	}
	virtual ~EngineRenderEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_EngineRender )
		EVENT_CLASS_CATEGORY( EC_Application )
};

class HObject;
class ObjectSelectedEvent : public Event
{
public:
	ObjectSelectedEvent()
	{
	}
	virtual ~ObjectSelectedEvent()
	{
	}

	inline HObject* GetSelectedObject() { return m_pSelectedActor; }
	inline void SetSelectedObject( HObject* pActor ) { m_pSelectedActor = pActor; }

	EVENT_CLASS_TYPE( ET_ObjectSelected )
		EVENT_CLASS_CATEGORY( EC_Engine )

protected:
	HObject* m_pSelectedActor;

};


// -----------------
//	Application Events
// -----------------

class AppEvent : public Event
{
public:
	AppEvent() 
	{
	}
	virtual ~AppEvent() 
	{
	}

};
