// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HSkeletalMeshComponent.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_Body = AddComponent<HSkeletalMeshComponent>( "Body" );
	m_Body->GetTransform().LinkTo( m_Transform );

}

ACharacter::~ACharacter()
{

}

