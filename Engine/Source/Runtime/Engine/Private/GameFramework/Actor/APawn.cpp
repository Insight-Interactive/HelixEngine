// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"

#include "Engine/Engine.h"
#include "GameFramework/Components/HControllerComponent.h"


APawn::APawn( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_Controller(NULL)
{
	m_Controller = AddComponent<HControllerComponent>( "Controller" );
}

APawn::~APawn()
{
}

void APawn::Move(const FVector3& Direction, const float Value)
{
	if (m_RootComponent != nullptr)
	{
		m_Velocity = m_MovementSpeed * Value * GEngine->GetDeltaTime();
		FVector3 Pos = m_RootComponent->GetPosition();
		Pos += Direction * m_Velocity;
		m_RootComponent->SetPosition(Pos);
	}
}

void APawn::MoveForward(float Value)
{
	if (m_RootComponent != nullptr)
		Move( m_RootComponent->GetLocalForward(), Value);
}

void APawn::MoveRight(float Value)
{
	if (m_RootComponent != nullptr)
		Move( m_RootComponent->GetLocalRight(), Value);
}

void APawn::MoveUp(float Value)
{
	if (m_RootComponent != nullptr)
		Move( m_RootComponent->GetLocalUp(), Value);
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
