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

class AppBeginPlayEvent : public Event
{
public:
	AppBeginPlayEvent()
	{
	}
	virtual ~AppBeginPlayEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_AppBeginPlay )
		EVENT_CLASS_CATEGORY( EC_Application )
};

class AppTickEvent : public Event
{
public:
	AppTickEvent()
	{
	}
	virtual ~AppTickEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_AppTick )
		EVENT_CLASS_CATEGORY( EC_Application )
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent()
	{
	}
	virtual ~AppRenderEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_AppRender )
		EVENT_CLASS_CATEGORY( EC_Application )
};
