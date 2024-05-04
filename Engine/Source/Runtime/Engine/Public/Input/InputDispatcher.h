// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	File: FInputDispatcher.h
	Source: FInputDispatcher.cpp

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
class FWindow;


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
using ActionPair		= std::pair<StringHashValue, EInputEvent>;

class Event;

class FInputDispatcher : public FSerializeableInterface
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class SceneViewportPanel;
public:
	FInputDispatcher();
	~FInputDispatcher();

	void Initialize( FWindow* pNativeWindow );
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

	FRawInputSurveyer& GetInputSureyor();

protected:
	void LoadMappingsFromFile( const Char* Filename );
	virtual void Serialize( const Char* Filepath ) override;
	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	/*
		Unregisters all callbacks registered to this input dispatcher.
	*/
	void FlushCallbacks();
	/*
		Set wether this dispatcher can call its bound callbacks.
	*/
	void SetCanDispatchListeners( bool CanDispatch );
	/*
		Returns true if this dispatcher can call callbacks attached too it.
	*/
	bool GetCanDispatchListeners() const;

	void AddActionMappingInternal( StringHashValue HintHash, DigitalInput MappedKey, const Char* HintName = NULL );
	void AddAxisMappingInternal( StringHashValue HintHash, DigitalInput MappedKey, float Scale, const Char* HintName = NULL );

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
	// Wether or not registered callbacks can be called.
	bool m_CanDispatchListeners;

	// Surveys the bound window for input. Driving the callback dispatch algorithm.
	FRawInputSurveyer m_InputSurveyer;

	// Holds all axis mapping profiles.
	std::vector<AxisMappingInternal> m_AxisMappings;

	// Holds all action mapping profiles.
	std::vector<ActionMappingInternal> m_ActionMappings;

	// Holds all callback funcitons and their corisponding hints found the Axis mappings stored in FInputDispatcher::m_AxisMappings.
	std::map<StringHashValue, std::vector<AxisCallback>> m_AxisCallbacks;

	// Holds all callback funcitons and their corisponding hints found the actoin mappings stored in FInputDispatcher::m_ActionMappings.
	std::map<ActionPair, std::vector<ActionCallback>> m_ActionCallbacks;

};


//
// Inline function implementations
//

FORCEINLINE void FInputDispatcher::SetCanDispatchListeners( bool CanDispatch )
{
	m_CanDispatchListeners = CanDispatch;
}

FORCEINLINE bool FInputDispatcher::GetCanDispatchListeners() const
{
	return m_CanDispatchListeners;
}

FORCEINLINE void FInputDispatcher::FlushCallbacks()
{
	m_AxisCallbacks.clear();
	m_ActionCallbacks.clear();
}

FORCEINLINE void FInputDispatcher::AddActionMapping( ActionMapping& Mapping )
{
	AddActionMappingInternal( StringHash( Mapping.Hint ), Mapping.MappedKeyCode
#if HE_WITH_EDITOR
		, Mapping.Hint 
#endif
	);
}

FORCEINLINE void FInputDispatcher::AddAxisMapping( AxisMapping& Mapping )
{
	AddAxisMappingInternal( StringHash( Mapping.Hint ), Mapping.MappedKeycode, Mapping.Scale, Mapping.Hint );
}

FORCEINLINE void FInputDispatcher::AddActionMapping( const char* Hint, DigitalInput MappedKey )
{
	AddActionMappingInternal( StringHash( Hint ), MappedKey
#if HE_WITH_EDITOR
		, Hint
#endif
	);
}

FORCEINLINE void FInputDispatcher::AddAxisMapping( const char* Hint, DigitalInput MappedKey, float Scale )
{
	AddAxisMappingInternal( StringHash( Hint ), MappedKey, Scale
#if HE_WITH_EDITOR
		, Hint 
#endif
	);
}

FORCEINLINE void FInputDispatcher::AddActionMappingInternal(StringHashValue HintHash, DigitalInput MappedKey, const Char* HintName )
{
	ActionMappingInternal Mapping(HintHash, MappedKey);
#if HE_WITH_EDITOR
	strcpy_s( Mapping.HintName, HintName );
#endif
	m_ActionMappings.emplace_back( Mapping );
}

FORCEINLINE void FInputDispatcher::AddAxisMappingInternal(StringHashValue HintHash, DigitalInput MappedKey, float Scale, const Char* HintName )
{
	AxisMappingInternal Mapping( HintHash, MappedKey, Scale );
#if HE_WITH_EDITOR
	strcpy_s( Mapping.HintName, HintName );
#endif
	m_AxisMappings.emplace_back( Mapping );
}

FORCEINLINE void FInputDispatcher::RegisterAxisCallback( const char* MappedHint, AxisCallback Callback )
{
	m_AxisCallbacks[StringHash( MappedHint )].push_back( Callback );
}

FORCEINLINE void FInputDispatcher::RegisterActionCallback( const char* MappedHint, EInputEvent EventType, ActionCallback Callback )
{
	m_ActionCallbacks[{StringHash( MappedHint ), EventType}].push_back( Callback );
}

template <typename EventType, typename ... EventInitArgs>
FORCEINLINE void FInputDispatcher::ProcessInputEventEx( EventInitArgs ... Args )
{
	ProcessInputEvent( EventType( Args... ) );
}

FORCEINLINE FRawInputSurveyer& FInputDispatcher::GetInputSureyor()
{
	return m_InputSurveyer;
}
