// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
	File: InputDispatcher.h
	Source: InputDispatcher.cpp

	Author: Garrett Courtney

	Description:
	Manages client facing input bindings. A user can choose to bind an axis/action to a key via a mapping,
	then recieve callbacks when that mappings key is pressed.

*/
#pragma once

#include "Callbacks.h"
#include "Input/RawInput.h"


class AxisEvent;
class ActionEvent;
class KeyPressedEvent;
class MouseScrolledEvent;
class InputEvent;


/*
	Describes an axis mapping that input components can use to recieve events on.
	@param Hint - The name to associate an event with.
	@param MappedKeyCode - The keycode that will trigger the event callbacks attached to this axis mapping.
	@param Scale - A value to scale the raw value that is passed to callback functions.
*/
struct AxisMapping
{
	const char*		Hint;
	DigitalInput	MappedKeycode;
	float			Scale;
};

/*
	Describes an action mapping that input components can use to recieve events on.
	@param Hint - The name to associate an event with.
	@param MappedKeyCode - The keycode that will trigger the event callbacks attached to this axis mapping.
*/
struct ActionMapping
{
	const char*		Hint;
	DigitalInput	MappedKeyCode;
};

using ActionCallback	= std::function<void( void )>;
using AxisCallback		= std::function<void( float )>;
using ActionPair		= std::pair<int32, EInputEvent>;

class Event;

class InputDispatcher
{
public:
	InputDispatcher();
	~InputDispatcher();

	void Initialize( void* pNativeWindow );
	void UnInitialize();

	/*
		Updates the keyboard axis mappings with the OS.
	*/
	void UpdateInputs( float DeltaMs );

	/*
		See AddActionMapping(...) and/or ActionMapping struct.
	*/
	void AddActionMapping(ActionMapping& Mapping);

	/*
		See AddAxisMapping(...) and/or AxisMapping struct.
	*/
	void AddAxisMapping( AxisMapping& Mapping );

	/*
		Adds an action mapping for callbacks to request by name.
	*/
	void AddActionMapping(const char* Hint, DigitalInput MappedKey );

	/*
		Adds an action mapping for callbacks to request by name.
	*/
	void AddAxisMapping( const char* Hint, DigitalInput MappedKey, float Scale);

	/*
		Register an Axis callback function.
	*/
	void RegisterAxisCallback( const char* MappedHint, AxisCallback Callback );

	/*
		Register an Action callback function.
	*/
	void RegisterActionCallback( const char* MappedHint, EInputEvent EventType, ActionCallback Callback );

	/*
		Decodes and sends events to approbriate Dispatch* functions.
	*/
	void ProcessInputEvent( Event& e );

	RawInputSurveyer& GetInputSureyor();

private:
	void AddActionMappingInternal( int32 HintHash, DigitalInput MappedKey );
	void AddAxisMappingInternal( int32 HintHash, DigitalInput MappedKey, float Scale );

	/*
		Extension of "ProcessInputEvent" that allows the function call and event declaration to be declared inline.
	*/
	template <typename EventType, typename ... EventInitArgs>
	void ProcessInputEventEx( EventInitArgs ... Args );

	/*
		Dispatch an event to all callbacks that request a key press event.
	*/
	bool DispatchAxisEvent( AxisEvent& e );
	
	/*
		Dispatch an event to all callbacks that request an action event.
	*/
	bool DispatchActionEvent( ActionEvent& e );

private:
	/*
		Internal non-client facing axis mapping representation.
	*/
	struct AxisMappingInternal
	{
		AxisMappingInternal( int32& Hash, DigitalInput& Key, float& AxisScale )
			: HintHash( Hash ), MappedKeycode( Key ), Scale( AxisScale )
		{
		}
		int32			HintHash;
		DigitalInput	MappedKeycode;
		float			Scale;
	};

	/*
		Internal non-client facing axis mapping representation.
	*/
	struct ActionMappingInternal
	{
		ActionMappingInternal( int32& Hash, DigitalInput& Key )
			: HintHash( Hash ), MappedKeycode( Key )
		{
		}
		int32			HintHash;
		DigitalInput	MappedKeycode;
	};

private:
	// Surveys the bound window for input. Driving the callback dispatch algorithm.
	RawInputSurveyer m_InputSurveyer;

	// Holds all axis mapping profiles.
	std::vector<AxisMappingInternal> m_AxisMappings;

	// Holds all action mapping profiles.
	std::vector<ActionMappingInternal> m_ActionMappings;

	// Holds all callback funcitons and their corisponding hints found the Axis mappings stored in InputDispatcher::m_AxisMappings.
	std::map<int32, std::vector<AxisCallback>> m_AxisCallbacks;

	// Holds all callback funcitons and their corisponding hints found the actoin mappings stored in InputDispatcher::m_ActionMappings.
	std::map<ActionPair, std::vector<ActionCallback>> m_ActionCallbacks;

};


//
// Inline function implementations
//


FORCEINLINE void InputDispatcher::AddActionMapping( ActionMapping& Mapping )
{
	AddActionMappingInternal( StringHash( Mapping.Hint, strlen( Mapping.Hint ) ), Mapping.MappedKeyCode );
}

FORCEINLINE void InputDispatcher::AddAxisMapping( AxisMapping& Mapping )
{
	AddAxisMappingInternal( StringHash( Mapping.Hint, strlen( Mapping.Hint ) ), Mapping.MappedKeycode, Mapping.Scale );
}

FORCEINLINE void InputDispatcher::AddActionMapping( const char* Hint, DigitalInput MappedKey )
{
	AddActionMappingInternal( StringHash( Hint, strlen( Hint ) ), MappedKey );
}

FORCEINLINE void InputDispatcher::AddAxisMapping( const char* Hint, DigitalInput MappedKey, float Scale )
{
	AddAxisMappingInternal( StringHash( Hint, strlen( Hint ) ), MappedKey, Scale );
}

FORCEINLINE void InputDispatcher::AddActionMappingInternal( int32 HintHash, DigitalInput MappedKey )
{
	m_ActionMappings.emplace_back( HintHash, MappedKey );
}

FORCEINLINE void InputDispatcher::AddAxisMappingInternal( int32 HintHash, DigitalInput MappedKey, float Scale )
{
	m_AxisMappings.emplace_back( HintHash, MappedKey, Scale );
}

FORCEINLINE void InputDispatcher::RegisterAxisCallback( const char* MappedHint, AxisCallback Callback )
{
	m_AxisCallbacks[StringHash( MappedHint, strlen( MappedHint ) )].push_back( Callback );
}

FORCEINLINE void InputDispatcher::RegisterActionCallback( const char* MappedHint, EInputEvent EventType, ActionCallback Callback )
{
	m_ActionCallbacks[{StringHash( MappedHint, strlen( MappedHint ) ), EventType}].push_back( Callback );
}

template <typename EventType, typename ... EventInitArgs>
FORCEINLINE void InputDispatcher::ProcessInputEventEx( EventInitArgs ... Args )
{
	ProcessInputEvent( EventType( Args... ) );
}

FORCEINLINE RawInputSurveyer& InputDispatcher::GetInputSureyor()
{
	return m_InputSurveyer;
}