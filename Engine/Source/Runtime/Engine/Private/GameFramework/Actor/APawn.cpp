// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"
#include "World/World.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"
#include "GameFramework/Components/HCubeColliderComponent.h"
#include "Engine/Engine.h"


APawn::APawn( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_CameraPitchSpeedMultiplier(kDefaultCameraPitchSpeedMultiplier)
	, m_CameraYawSpeedMultiplier(kDefaultCameraYawSpeedMultiplier)
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_pController(NULL)
{
	m_pController = AddComponent<HControllerComponent>(TEXT("Player Controller"));
	m_pRootComponent = AddComponent<HCapsuleColliderComponent>( TEXT( "Character Bounds" ) );
	SetRootComponent(m_pRootComponent);

	((HCapsuleColliderComponent*)m_pRootComponent)->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_X, true );
	((HCapsuleColliderComponent*)m_pRootComponent)->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Y, true );
	((HCapsuleColliderComponent*)m_pRootComponent)->GetRigidBody().ToggleConstrainAxis( HRigidBody::MA_Z, true );
}

APawn::~APawn()
{
}

void APawn::Move(const FVector3& Direction, const float Value)
{
	m_Velocity = m_MovementSpeed * Value * GetWorld()->GetDeltaTime();
	FVector3 Pos = m_pRootComponent->GetPosition();
	Pos += Direction * m_Velocity;
	m_pRootComponent->SetPosition(Pos);
}

void APawn::MoveForward(float Value)
{
	Move(m_pRootComponent->GetLocalForward(), Value);
}

void APawn::MoveRight(float Value)
{
	Move(m_pRootComponent->GetLocalRight(), Value);
}

void APawn::MoveUp(float Value)
{
	Move(m_pRootComponent->GetLocalUp(), Value);
}

void APawn::Sprint()
{
	m_bIsSprinting = !m_bIsSprinting;
	if (m_bIsSprinting)
	{
		m_MovementSpeed = m_SprintSpeed;
	}
	else
	{
		m_MovementSpeed = kDefaultMovementSpeed;
	}
}
