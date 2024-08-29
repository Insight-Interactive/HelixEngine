// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_RootComponent = AddComponent<HSceneComponent>( "CharacterRoot" );
}

ACharacter::~ACharacter()
{

}

