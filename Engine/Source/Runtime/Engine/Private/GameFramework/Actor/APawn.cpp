// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"
#include "World/HWorld.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "Engine/HEngine.h"


APawn::APawn( HWorld* pWorld, const HName& Name )
	: AActor( pWorld, Name )
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
	float DT = GEngine->GetDeltaTime();
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

/*
	Actual: m_FrameTime == 0.f in all cases except when accessed from Engine class.
	Expected: m_FrameTime > 0.f (Or some other value. It is the time between frames).

	// Engine.h
	//
	// FrameTimeManager is a private class inside Engine class
	FrameTimeManager m_FrameTimeManager; // m_FrameTime is private inside FrameTimeManager class.


	// Engine.cpp
	//
	void Engine::Update()
	{
		while( App::IsOpen() )
		{
			float DeltaTime = m_TimeManager.GetFrameTime(); // Returns the propper value! But calling GEngine->GetDeltaTime() does not!
			float AnotherDeltaTime = GEngine->GetDeltaTime(); // Returns the proper value. But calling in Pawn::Move does not.
			...
			m_TimeManager.Update(); // Calculates the actual frame time and stores it in Engine::FrameTimeManager::m_FrameTime. Usually it is around 2.5f.
		}
	}

	// Engine.h
	//
	inline float Engine::FrameTimeManager::GetFrameTime() const
	{
		return m_FrameTime; // Updated in Engine::FrameTimeManager::Update. Debugging in the update loop gives expected values.
		// NOTE: Debugging from Pawn::Move to here also shows the value to be zero.
	}

	// Engine.h
	//
	inline float Engine::GetDeltaTime() const 
	{
		return m_TimeManager.GetFrameTime();
	}

	// World.cpp
	//
	float World::GetDeltaTime() const
	{
		return GEngine->GetDeltaTime(); // GEngine is a global pointer to the instance of the Engine.
	}

	// Actor.h
	//
	inline World* Actor::GetWorld()
	{
		return pWorld; // Cached pointer to the world this actor is in. Assigned in constructor, cannot be null.
	}

	// Pawn.cpp
	//
	void Pawn::Move() // Pawn derives off of Actor.
	{
		float DT = GEngine->GetDeltaTime(); // Skipping going through all the other classes and just calling directly, also returns 0.f but using the same call from Engine::Update returns the expected value.
		float DeltaTime = GetWorld()->GetDeltaTime(); // <-- 0.0f?
	}

	Also Debuggging GEngine->m_TimeManager.m_FrameTime shows values of 0.f but debugging the same value inide Engine::Update shows the proper value.

*/
