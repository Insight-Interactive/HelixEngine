// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/GameInstance.h"

#include "Engine/Event/EngineEvent.h"


HGameInstance::HGameInstance()
{
}

HGameInstance::~HGameInstance()
{
}

void HGameInstance::OnGameSetFocus()
{

}

void HGameInstance::OnGameLostFocus()
{

}

void HGameInstance::OnEvent( EngineEvent& e )
{
	EventDispatcher Dispatcher( e );

	// Engine
	Dispatcher.Dispatch<EngineBeginPlayEvent>( this, &HGameInstance::OnEnginePostStartup );
}

bool HGameInstance::OnEnginePostStartup( EngineBeginPlayEvent& e )
{
	GetWorld()->GetOwningViewport()->GetInputDispatcher()->RegisterActionCallback( "Menu", IE_Pressed, std::bind(&HGameInstance::ToggleMenu, this));
	return false;
}

void HGameInstance::ToggleMenu()
{

}
