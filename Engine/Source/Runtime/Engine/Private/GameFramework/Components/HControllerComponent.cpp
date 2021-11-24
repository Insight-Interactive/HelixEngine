// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HControllerComponent.h"

HControllerComponent::HControllerComponent(const HName& Name)
	: HActorComponent( Name )
{

}

HControllerComponent::~HControllerComponent()
{

}

void HControllerComponent::AddGamepadVibration(uint32 PlayerIndex, EGampadRumbleMotor Motor, float Amount)
{
	//FInputDispatcher::Get().AddGamepadVibration(PlayerIndex, Motor, Amount);
}

void HControllerComponent::Render(FCommandContext& GfxContext)
{

}
