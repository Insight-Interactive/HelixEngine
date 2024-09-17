// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HStaticMeshComponent.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_Body = AddComponent<HStaticMeshComponent>( "CharacterBody" );
	m_Body->GetTransform().LinkTo( m_Transform );

}

ACharacter::~ACharacter()
{

}

