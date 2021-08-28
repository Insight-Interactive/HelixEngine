#include "EnginePCH.h"

#include "Actor/Components/HControllerComponent.h"

void HControllerComponent::AddGamepadVibration(uint32 PlayerIndex, EGampadRumbleMotor Motor, float Amount)
{
	//InputDispatcher::Get().AddGamepadVibration(PlayerIndex, Motor, Amount);
}

void HControllerComponent::Render(ICommandContext& GfxContext)
{

}
