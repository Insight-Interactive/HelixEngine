// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HControllerComponent.h"

HControllerComponent::HControllerComponent(FComponentInitArgs& InitArgs)
	: HActorComponent( InitArgs )
{

}

HControllerComponent::~HControllerComponent()
{

}

void HControllerComponent::AddGamepadVibration(uint32 PlayerIndex, EGampadRumbleMotor Motor, float Amount)
{
	//FInputDispatcher::Get().AddGamepadVibration(PlayerIndex, Motor, Amount);
}
