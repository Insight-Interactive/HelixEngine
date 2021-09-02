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

	AddAxisMapping( "MoveForward", Key_W, 1.0f );
	AddAxisMapping( "MoveForward", Key_S, -1.0f );
	AddAxisMapping( "MoveForward", AnalogLeftStickY, 1.0f );
	AddAxisMapping( "MoveRight", Key_D, 1.0f );
	AddAxisMapping( "MoveRight", Key_A, -1.0f );
	AddAxisMapping( "MoveRight", AnalogLeftStickX, 1.0f );
	AddAxisMapping( "MoveUp", Key_E, 1.0f );
	AddAxisMapping( "MoveUp", Key_Q, -1.0f );
	AddAxisMapping( "MoveUp", AnalogRightTrigger, -1.0f );
	AddAxisMapping( "MoveUp", AnalogLeftTrigger, 1.0f );
	AddAxisMapping( "LookUp", AnalogMouseY, 1.0f );
	AddAxisMapping( "LookUp", AnalogRightStickY, 1.0f );
	AddAxisMapping( "LookRight", AnalogMouseX, 1.0f );
	AddAxisMapping( "LookRight", AnalogMouseX, -1.0f );
	AddAxisMapping( "LookRight", AnalogRightStickX, 1.0f );

	AddActionMapping( "Sprint", Key_LShift );
}

void InputDispatcher::UnInitialize()
{
	m_InputSurveyer.UnInitialize();
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
