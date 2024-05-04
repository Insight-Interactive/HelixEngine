// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"
#include "World/World.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "Engine/Engine.h"


APawn::APawn( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_pController(NULL)
{
	m_pController = AddComponent<HControllerComponent>(TEXT("Controller"));
}

APawn::~APawn()
{
}

void APawn::Move(const FVector3& Direction, const float Value)
{
	if (m_pRootComponent != nullptr)
	{
		m_Velocity = m_MovementSpeed * Value * (float)GEngine->GetDeltaTime();
		FVector3 Pos = m_pRootComponent->GetPosition();
		Pos += Direction * m_Velocity;
		m_pRootComponent->SetPosition(Pos);
	}
}

void APawn::MoveForward(float Value)
{
	if (m_pRootComponent != nullptr)
		Move(m_pRootComponent->GetLocalForward(), Value);
}

void APawn::MoveRight(float Value)
{
	if (m_pRootComponent != nullptr)
		Move(m_pRootComponent->GetLocalRight(), Value);
}

void APawn::MoveUp(float Value)
{
	if (m_pRootComponent != nullptr)
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
