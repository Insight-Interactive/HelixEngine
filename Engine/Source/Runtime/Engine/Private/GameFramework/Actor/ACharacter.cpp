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
	if (!InitArgs.bDisableCollision)
	{
		m_CharacterBounds = AddComponent<HCapsuleColliderComponent>( "CharacterBounds" );
		m_CharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_X, true );
		m_CharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Y, true );
		m_CharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Z, true );

		SetRootComponent( m_CharacterBounds );
	}
}

ACharacter::~ACharacter()
{

}

