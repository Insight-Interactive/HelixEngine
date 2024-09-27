// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Input/InputEnums.h"
#include "Callbacks.h"


class Event;
using EventCallbackFn = std::function<void( Event& )>;

enum class EEventType
{
	ET_None = 0,
	// FWindow
	ET_WindowClose, ET_WindowResize, ET_WindowFocus, ET_WindowLostFocus, ET_ToggleWindowFullScreen, ET_WindowFileDrop, ET_WindowMaximized, ET_WindowMinimized,
	// Application
	ET_AppSuspending, ET_AppResuming,
	// Engine
	ET_ObjectSelected, ET_EnginePreStartup, ET_EngineStartup, ET_EnginePostStartup, ET_EngineBeginPlay, ET_EngineEndPlay, ET_EngineTick, ET_EngineRender,
	// Input
	ET_InputAction, ET_InputAxis,
	// Key
	ET_KeyPressed, ET_KeyReleased, ET_KeyTyped, ET_KeyHeld,
	// Mouse
	ET_MouseButtonPressed, ET_MouseButtonReleased, ET_MouseMoved, ET_RawMouseMoved, ET_MouseScrolled,
	// Gamepad
	ET_GamepadThumbstickMoved,
	// Editor
	ET_ContentItemDoubleClicked, ET_EditorTabOpened, ET_EditorTabClosed,
	//Platform,
	ET_PlatformWindowsProcEvent,

	ET_EventCount,
};

enum EEventCategory
{
	EC_None			= 0x0000,
	EC_Application	= 0x0001,
	EC_Input		= 0x0002,
	EC_Keyboard		= 0x0004,
	EC_Gamepad		= 0x0008,
	EC_Mouse		= 0x0010,
	EC_MouseButton	= 0x0020,
	EC_Physics		= 0x0040,
	EC_Translation	= 0x0080,
	EC_Connection	= 0x0100,
	EC_Network		= 0x0200,
	EC_Window		= 0x0400,
	EC_Renderer		= 0x0800,
	EC_Engine		= 0x1000,
	EC_Editor		= 0x2000,
};

#define EVENT_CLASS_TYPE( Type )	static EEventType GetStaticType()	{ return EEventType::##Type; }				\
									virtual EEventType GetEventType()	const override { return GetStaticType(); }	\
									virtual const char* GetName()		const override { return #Type; }

#define EVENT_CLASS_CATEGORY( Category ) virtual int GetCategoryFlags() const override { return Category; }


/*
	Describes an event that can be broadcasted by an event dispatcher.
*/
class Event
{
	friend class EventDispatcher;
public:
	virtual const char* GetName()		const = 0;
	virtual int GetCategoryFlags()		const = 0;
	virtual EEventType GetEventType()	const = 0;
	virtual String ToString()			const { return GetName(); }

	inline bool IsHandled()								const	{ return m_IsHandled; }
	inline void SetIsHandled(bool Value)						{ m_IsHandled = Value; }
	inline bool IsInCategory( EEventCategory category )			{ return ( GetCategoryFlags() & category ); }

protected:
	Event()
	{
	}
	virtual ~Event()
	{
	}

protected:
	bool m_IsHandled = false;

};


/*
	Describes a input event.
*/
class InputEvent
{
public:
	inline DigitalInput GetKeyCode()	const { return m_KeyMapCode; }
	inline EInputEvent GetInputEventType()	const { return m_Status; }

	String  ToString() const
	{
		std::stringstream ss;
		ss << "Input Event: (Button) " << m_KeyMapCode << " (Status) " << m_Status;
		return ss.str();
	}

protected:
	InputEvent( DigitalInput KeyMapCode, EInputEvent Status )
		: m_KeyMapCode( KeyMapCode )
		, m_Status( Status )
	{
	}
	virtual ~InputEvent()
	{
	}

	DigitalInput m_KeyMapCode;
	EInputEvent m_Status;
};


class ActionEvent : public Event, public InputEvent
{
public:
	ActionEvent( DigitalInput KeyMapCode, EInputEvent Status )
		: InputEvent( KeyMapCode, Status )
	{
	}
	virtual ~ActionEvent()
	{
	}

	EVENT_CLASS_TYPE( ET_InputAction )
		EVENT_CLASS_CATEGORY( EC_Input )

};


class AxisEvent : public Event, public InputEvent
{
public:
	AxisEvent( DigitalInput KeyMapCode, float Delta )
		: InputEvent( KeyMapCode, IE_Moved )
		, m_Delta( Delta )
	{
	}
	virtual ~AxisEvent()
	{
	}

	inline float GetMoveDelta() const { return m_Delta; }

	EVENT_CLASS_TYPE( ET_InputAxis )
		EVENT_CLASS_CATEGORY( EC_Input )

private:
	float m_Delta;

};


/*
	Dispatches events to bound functions.
*/
class EventDispatcher
{
public:
	EventDispatcher( Event& event )
		: m_Event( event ) 
	{
	}
	~EventDispatcher()
	{
	}

	/*
		Calls the bound function with the specified event as the input parameter.
		Ex) Dispach<MyEvent>( this, &SomeClass::SomeMemberFn );
	*/
	template <typename EventType, typename ClassTarget>
	bool Dispatch( ClassTarget* pClass, bool(ClassTarget::* pMemberFn)(EventType&) )
	{
		if (m_Event.GetEventType() == EventType::GetStaticType())
		{
			m_Event.m_IsHandled = HE_INVOKE_MEMBER_FN( pClass, pMemberFn, *(EventType*)&m_Event );
			return true;
		}
		return false;
	}

private:
	Event& m_Event;

};


//
// Inline function implementations
//

inline std::ostream& operator<<( std::ostream& os, const Event& e )
{
	return os << e.ToString();
}
