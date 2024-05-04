// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"
#include "Transform.h"


class HControllerComponent;

static const float kDefaultMovementSpeed = 32.f;
static const float kDefaultSprintSpeed = kDefaultMovementSpeed * 3.f;

HCLASS()
class APawn : public AActor
{
	using Super = AActor;
public:
	HE_GENERATED_BODY( APawn )

		 
	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		SetupController(*m_pController);
	}

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Sprint();

	void SetMovementSpeed(float Value);
	float GetMovementSpeed() const;

protected:
	virtual void SetupController(HControllerComponent& Controller)
	{
		/* Override for behavior. */ 
	}


protected:
	void Move(const FVector3& Direction, const float Value);


protected:
	float m_MovementSpeed;
	float m_SprintSpeed;
	float m_Velocity;
	bool m_bIsSprinting;

	HControllerComponent* m_pController;

};


//
// Inline function implementations
//

inline void APawn::SetMovementSpeed( float Value )
{
	m_MovementSpeed = Value;
}

inline float APawn::GetMovementSpeed() const
{
	return m_MovementSpeed;
}
