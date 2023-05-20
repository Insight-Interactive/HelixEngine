// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"
#include "Transform.h"


class HControllerComponent;

static const float kDefaultMovementSpeed = 32.f;
static const float kDefaultSprintSpeed = kDefaultMovementSpeed * 3.f;
static const float kDefaultCameraPitchSpeedMultiplier = 50.f;
static const float kDefaultCameraYawSpeedMultiplier = 50.f;

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
	float GetVerticalLookSpeed() const;
	float GetHorizontalLookSpeed() const;
	void SetVerticalLookSpeed( float Speed );
	void SetHorizontalLookSpeed( float Speed );

protected:
	virtual void SetupController(HControllerComponent& Controller)
	{
		/* Override for behavior. */ 
	}


protected:
	void Move(const FVector3& Direction, const float Value);


protected:
	float m_CameraPitchSpeedMultiplier;
	float m_CameraYawSpeedMultiplier;
	float m_MovementSpeed;
	float m_SprintSpeed;
	float m_Velocity;
	bool m_bIsSprinting;
	HControllerComponent* m_pController;
	HSceneComponent* m_pRootComponent;

};


//
// Inline function implementations
//

inline void APawn::SetMovementSpeed(float Value)
{
	m_MovementSpeed = Value;
}

inline float APawn::GetVerticalLookSpeed() const
{
	return m_CameraPitchSpeedMultiplier;
}

inline float APawn::GetHorizontalLookSpeed() const
{
	return m_CameraYawSpeedMultiplier;
}

inline void APawn::SetVerticalLookSpeed( float Speed )
{
	m_CameraPitchSpeedMultiplier = Speed;
}

inline void APawn::SetHorizontalLookSpeed( float Speed )
{
	m_CameraYawSpeedMultiplier = Speed;
}

inline float APawn::GetMovementSpeed() const
{
	return m_MovementSpeed;
}
