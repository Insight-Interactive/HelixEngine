// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	if (!InitArgs.bDisableCollision)
	{
		m_CharacterBounds = AddComponent<HCapsuleColliderComponent>( "CharacterBounds" );
		HRigidBody& rb = m_CharacterBounds->GetRigidBody();
		rb.SetIsKinematic( true );

		SetRootComponent( m_CharacterBounds );
	}
}

ACharacter::~ACharacter()
{

}

