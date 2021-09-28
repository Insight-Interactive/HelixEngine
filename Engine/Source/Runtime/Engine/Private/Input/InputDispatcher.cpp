#include "EnginePCH.h"

#include "Input/InputDispatcher.h"

#include "Input/MouseEvent.h"
#include "Input/KeyEvent.h"
#include "Input/GamepadEvents.h"
#include "Input/InputEnums.h"
#include "FileSystem.h"


InputDispatcher::InputDispatcher()
{
}

InputDispatcher::~InputDispatcher()
{
	UnInitialize();
}

void InputDispatcher::Initialize( void* pNativeWindow )
{
	m_InputSurveyer.Initialize( pNativeWindow );
}

void InputDispatcher::UnInitialize()
{
	m_InputSurveyer.UnInitialize();
}

void InputDispatcher::Serialize( const Char* Filepath )
{
	rapidjson::StringBuffer StrBuffer;
	WriteContext Writter( StrBuffer );

	Writter.StartObject();
	{
		Serialize( Writter );
	}
	Writter.EndObject();

	FileRef OutFile( Filepath, FUM_Write, CM_Text );
	HE_ASSERT( OutFile->IsOpen() );
	if (OutFile->IsOpen())
	{
		if (!OutFile->WriteData( (void*)StrBuffer.GetString(), StrBuffer.GetSize(), 1 ))
		{
			HE_LOG( Error, TEXT( "Failed to serialize input dispatcher mappings!" ) );
			HE_ASSERT( false );
		}
	}
}

void InputDispatcher::Serialize( WriteContext& Output )
{
#if HE_WITH_EDITOR // In client shipping builds, the user should never be able to serialize new input mappings.
	Output.Key( "AxisMappings" );
	Output.StartArray();
	{
		for (size_t i = 0; i < m_AxisMappings.size(); ++i)
		{
			Output.StartObject();
			{
				auto Mapping = m_AxisMappings[i];
				Output.Key( "Name" );
				Output.String( Mapping.HintName );

				Output.Key( "Key" );
				Output.Int( (int)Mapping.MappedKeycode );

				Output.Key( "Scale" );
				Output.Double( Mapping.Scale );
			}
			Output.EndObject();
		}
	}
	Output.EndArray();

	Output.Key( "ActionMappings" );
	Output.StartArray();
	{
		for (size_t i = 0; i < m_ActionMappings.size(); ++i)
		{
			Output.StartObject();
			{
				auto Mapping = m_ActionMappings[i];
				Output.Key( "Name" );
				Output.String( Mapping.HintName );

				Output.Key( "Key" );
				Output.Int( (int)Mapping.MappedKeycode );
			}
			Output.EndObject();
		}
	}
	Output.EndArray();
#endif // HE_WITH_EDITOR
}

void InputDispatcher::Deserialize( const ReadContext& Value )
{
	// Add the axis mappings
	const rapidjson::Value& AxisMappingsArray = Value["AxisMappings"];
	for (rapidjson::SizeType i = 0; i < AxisMappingsArray.Size(); ++i)
	{
		const rapidjson::Value& AxisMapping = AxisMappingsArray[i];

		Char Name[64];
		JsonUtility::GetString( AxisMapping, "Name", Name, sizeof( Name ) );

		int32 Key = 0;
		JsonUtility::GetInteger( AxisMapping, "Key", Key );

		float Scale;
		JsonUtility::GetFloat( AxisMapping, "Scale", Scale );

		AddAxisMapping( Name, (DigitalInput)Key, Scale );
	}

	// Add the action mappings
	const rapidjson::Value& ActionMappingsArray = Value["ActionMappings"];
	for (rapidjson::SizeType i = 0; i < ActionMappingsArray.Size(); ++i)
	{
		const rapidjson::Value& ActionMapping = ActionMappingsArray[i];

		Char Name[64];
		JsonUtility::GetString( ActionMapping, "Name", Name, sizeof( Name ) );

		int32 Key = 0;
		JsonUtility::GetInteger( ActionMapping, "Key", Key );

		AddActionMapping( Name, (DigitalInput)Key );
	}
}

void InputDispatcher::LoadMappingsFromFile( const Char* Filename )
{
	rapidjson::Document MappingsJsonDoc;
	FileRef MappingsSource( Filename, FUM_Read );
	HE_ASSERT( MappingsSource->IsOpen() );
	JsonUtility::LoadDocument( MappingsSource, MappingsJsonDoc );
	if (MappingsJsonDoc.IsObject())
	{
		Deserialize( MappingsJsonDoc );
	}
}

void InputDispatcher::UpdateInputs( float DeltaTime )
{
	// Update the input devices.
	//
	m_InputSurveyer.Update( DeltaTime );

	// Iterate over all the action mappings and determine if any events need to be dispatched.
	//
	for (size_t i = 0; i < m_ActionMappings.size(); i++)
	{
		DigitalInput ActionKey = m_ActionMappings[i].MappedKeycode;

		if (m_InputSurveyer.IsFirstPressed( ActionKey ))
		{
			ProcessInputEventEx<ActionEvent>( ActionKey, IE_Pressed );
		}
		if (m_InputSurveyer.IsPressed( ActionKey ))
		{
			ProcessInputEventEx<ActionEvent>( ActionKey, IE_Held );
		}
		if (m_InputSurveyer.IsFirstReleased( ActionKey ))
		{
			ProcessInputEventEx<ActionEvent>( ActionKey, IE_Released );
		}
	}

	// Iterate over all the axis mappings and determine if any events need to be dispatched.
	//
	for (size_t i = 0; i < m_AxisMappings.size(); i++)
	{
		DigitalInput AxisKey = m_AxisMappings[i].MappedKeycode;

		if (IsValidAnalogInput( AxisKey ))
		{
			float MoveValue = m_InputSurveyer.GetAnalogInput( AxisKey );

			if (IsAnalogInputMoved( MoveValue ))
			{
				if (IsMouseMoveAnalog( AxisKey ))
				{
					ProcessInputEventEx<AxisEvent>( AxisKey, MoveValue );
				}
				else if (IsValidGamepadThumbStickAnalog( AxisKey ))
				{
					ProcessInputEventEx<AxisEvent>( AxisKey, MoveValue );
				}
				else if (IsValidGamepadTriggerAnalog( AxisKey ))
				{
					ProcessInputEventEx<AxisEvent>( AxisKey, MoveValue );
				}
			}
		}
		else
		{
			if (m_InputSurveyer.IsPressed( AxisKey ))
			{
				ProcessInputEventEx<AxisEvent>( AxisKey, 1.f );
			}
			else if (m_InputSurveyer.IsFirstReleased( AxisKey ))
			{
				ProcessInputEventEx<AxisEvent>( AxisKey, 0.f );
			}
		}
	}
}

void InputDispatcher::ProcessInputEvent( Event& e )
{
	EventDispatcher Dispatcher( e );

	Dispatcher.Dispatch<ActionEvent>( this, &InputDispatcher::DispatchActionEvent );
	Dispatcher.Dispatch<AxisEvent>( this, &InputDispatcher::DispatchAxisEvent );
}

bool InputDispatcher::DispatchAxisEvent( AxisEvent& e )
{
	for (AxisMappingInternal& Axis : m_AxisMappings)
	{
		// Find the key in the axis map.
		if (Axis.MappedKeycode == e.GetKeyCode() && !e.IsHandled())
		{
			e.SetIsHandled( true );

			// Use the keycode as the key into the axis map to find 
			// the callbacks associated with it.
			auto Callbacks = &m_AxisCallbacks[Axis.HintHash];
			for (AxisCallback Callback : *Callbacks)
			{
				// Call the callbacks.
				Callback( Axis.Scale * e.GetMoveDelta() );
			}
		}
	}
	return false;
}

bool InputDispatcher::DispatchActionEvent( ActionEvent& e )
{
	for (ActionMappingInternal& Action : m_ActionMappings)
	{
		if (Action.MappedKeycode == e.GetKeyCode())
		{
			auto Callbacks = &m_ActionCallbacks[{Action.HintHash, e.GetInputEventType()}];
			for (ActionCallback Callback : *Callbacks)
			{
				// Invoke the callbacks.
				Callback();
			}
		}
	}
	return false;
}
