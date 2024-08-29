// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HActorComponent.h"
#include "Engine/ViewportContext.h"


static const float kThumbStickLeftSensitivity = 20.0f;
static const float kThumbStickRightSensitivity = 20.0f;

struct GamepadStickSensitivity
{
	float Left;
	float Right;
};

HCOMPONENT()
class HControllerComponent : public HActorComponent
{
public:
	HE_COMPONENT_GENERATED_BODY( HControllerComponent )

	/*
		Bind a function to an input axis event defined in the user settings.
		@param AxisName: The name of the axis to bind to.
		@param Callback: A pointer to the funciton to call once the Axis has been invoked.
	*/
	template <typename ClassTarget, typename MethodTarget>
	inline void BindAxis(const char* AxisName, ClassTarget* Class, void(MethodTarget::* Fn)(float));

	/*
		Bind a function to an input action event defined in the user settings.
		@param ActionName: The name of the action to bind to.
		@param EventType: An event type that must be satisfied before the Callback can be invoked.
		@param Callback: A pointer to the funciton to call once the action has been invoked.
	*/
	template <typename ClassTarget, typename MethodTarget>
	inline void BindAction(const char* ActionName, EInputEvent EventType, ClassTarget* Class, void(MethodTarget::* Fn)());

	/*
		Add vibration to a players gamepad.
		@param PlayerIndex: The index of the player to add vibration to.
		@param Motor: The motor to add vibration to. This can be either the left or right gamepad motor.
		@param Amount: A normalized value (0 - 1) that specifies the amount of vibration to add to the controller. 0 being no vibration and 1 being full vibration.
	*/
	void AddGamepadVibration(uint32 PlayerIndex, EGampadRumbleMotor Motor, float Amount);

protected:
	GamepadStickSensitivity m_GamepadSensitivity;

};


//
// Inline function implementations
//

template <typename ClassTarget, typename MethodTarget>
inline void HControllerComponent::BindAxis(const char* AxisName, ClassTarget* Class, void(MethodTarget::* Fn)(float))
{
	GetWorld()->GetOwningViewport()->GetInputDispatcher()->RegisterAxisCallback( AxisName, std::bind( Fn, Class, std::placeholders::_1 ) );
}

template <typename ClassTarget, typename MethodTarget>
inline void HControllerComponent::BindAction(const char* ActionName, EInputEvent EventType, ClassTarget* Class, void(MethodTarget::* Fn)())
{
	GetWorld()->GetOwningViewport()->GetInputDispatcher()->RegisterActionCallback( ActionName, EventType, std::bind(Fn, Class) );
}
