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
#include "AssetRegistry/SerializeableInterface.h"


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

class InputDispatcher : public SerializeableInterface
{
	friend class HEngine;
	friend class HEditorEngine;
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

protected:
	void LoadMappingsFromFile( const Char* Filename );
	virtual void Serialize( const Char* Filepath ) override;
	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

private:
	/*
		Unregisters all callbacks registered to this input dispatcher.
	*/
	void FlushCallbacks();
	
	void AddActionMappingInternal( int32 HintHash, DigitalInput MappedKey, const Char* HintName = NULL );
	void AddAxisMappingInternal( int32 HintHash, DigitalInput MappedKey, float Scale, const Char* HintName = NULL );

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
	static const uint32 kMaxHintNameLength = 32;
	/*
		Internal non-client facing axis mapping representation.
	*/
	struct AxisMappingInternal
	{
		AxisMappingInternal( int32& Hash, DigitalInput& Key, float& AxisScale )
			: HintHash( Hash ), MappedKeycode( Key ), Scale( AxisScale )
		{
		}
#if HE_WITH_EDITOR
		Char HintName[kMaxHintNameLength];
#endif
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
#if HE_WITH_EDITOR
		Char HintName[kMaxHintNameLength];
#endif
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

FORCEINLINE void InputDispatcher::FlushCallbacks()
{
	m_AxisCallbacks.clear();
	m_ActionCallbacks.clear();
}

FORCEINLINE void InputDispatcher::AddActionMapping( ActionMapping& Mapping )
{
	AddActionMappingInternal( StringHash( Mapping.Hint, strlen( Mapping.Hint ) ), Mapping.MappedKeyCode
#if HE_WITH_EDITOR
		, Mapping.Hint 
#endif
	);
}

FORCEINLINE void InputDispatcher::AddAxisMapping( AxisMapping& Mapping )
{
	AddAxisMappingInternal( StringHash( Mapping.Hint, strlen( Mapping.Hint ) ), Mapping.MappedKeycode, Mapping.Scale, Mapping.Hint );
}

FORCEINLINE void InputDispatcher::AddActionMapping( const char* Hint, DigitalInput MappedKey )
{
	AddActionMappingInternal( StringHash( Hint, strlen( Hint ) ), MappedKey
#if HE_WITH_EDITOR
		, Hint
#endif
	);
}

FORCEINLINE void InputDispatcher::AddAxisMapping( const char* Hint, DigitalInput MappedKey, float Scale )
{
	AddAxisMappingInternal( StringHash( Hint, strlen( Hint ) ), MappedKey, Scale
#if HE_WITH_EDITOR
		, Hint 
#endif
	);
}

FORCEINLINE void InputDispatcher::AddActionMappingInternal( int32 HintHash, DigitalInput MappedKey, const Char* HintName )
{
	ActionMappingInternal Mapping(HintHash, MappedKey);
#if HE_WITH_EDITOR
	strcpy_s( Mapping.HintName, HintName );
#endif
	m_ActionMappings.emplace_back( Mapping );
}

FORCEINLINE void InputDispatcher::AddAxisMappingInternal( int32 HintHash, DigitalInput MappedKey, float Scale, const Char* HintName )
{
	AxisMappingInternal Mapping( HintHash, MappedKey, Scale );
#if HE_WITH_EDITOR
	strcpy_s( Mapping.HintName, HintName );
#endif
	m_AxisMappings.emplace_back( Mapping );
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
