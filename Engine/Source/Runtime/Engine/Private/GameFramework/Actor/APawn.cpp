// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"
#include "World/HWorld.h"
#include "GameFramework/Actor/Components/HControllerComponent.h"


APawn::APawn( HWorld* pWorld )
	: AActor( pWorld )
	, m_CameraPitchSpeedMultiplier(kDefaultCameraPitchSpeedMultiplier)
	, m_CameraYawSpeedMultiplier(kDefaultCameraYawSpeedMultiplier)
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_pController(NULL)
	, m_Transform()
	, m_CurrentModementSpeed(0.f)
{
	m_pController = AddComponent<HControllerComponent>(TEXT("Player Controller"));
}

APawn::~APawn()
{
}

void APawn::Move(const FVector3& Direction, const float Value)
{
	m_Velocity = m_MovementSpeed * Value * GetWorld()->GetDeltaTime();
	FVector3 Pos = m_Transform.GetPosition();
	Pos += Direction * m_Velocity;
	m_Transform.SetPosition(Pos);
}

void APawn::MoveForward(float Value)
{
	Move(m_Transform.GetLocalForward(), Value);
}

void APawn::MoveRight(float Value)
{
	Move(m_Transform.GetLocalRight(), Value);
}

void APawn::MoveUp(float Value)
{
	Move(m_Transform.GetLocalUp(), Value);
}

void APawn::Sprint()
{
	m_bIsSprinting = !m_bIsSprinting;
	if (m_bIsSprinting)
	{
		m_CurrentModementSpeed = m_MovementSpeed;
	}
	else
	{
		m_CurrentModementSpeed = m_SprintSpeed;
	}
}
