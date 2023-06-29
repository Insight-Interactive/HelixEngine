// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_pCharacterBounds = AddComponent<HCapsuleColliderComponent>( TEXT( "CharacterBounds" ) );
	m_pCharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_X, true );
	m_pCharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Y, true );
	m_pCharacterBounds->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Z, true );

	SetRootComponent( m_pCharacterBounds );

	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "PlayerCamera" ) );
}

ACharacter::~ACharacter()
{

}

void ACharacter::BeginPlay()
{
	Super::BeginPlay();
	FVector3 PlayerStartPos( -4.8f, 10.f, -106.4f );
	m_pRootComponent->SetPosition( PlayerStartPos );
}

void ACharacter::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );

}
