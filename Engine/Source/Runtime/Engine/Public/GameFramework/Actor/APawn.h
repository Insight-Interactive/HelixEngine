// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"
#include "Transform.h"


class HControllerComponent;

static const float kDefaultMovementSpeed = 50.f;
static const float kDefaultSprintSpeed = kDefaultMovementSpeed * 2.f;
static const float kDefaultCameraPitchSpeedMultiplier = 80.f;
static const float kDefaultCameraYawSpeedMultiplier = 80.f;

class APawn : public AActor
{
	using Super = AActor;
public:
	HE_GENERATED_BODY( APawn )


	virtual void BeginPlay() override
	{
		SetupController(*m_pController);
	}

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Sprint();

	inline void SetMovementSpeed(float Value);
	inline float GetMovementSpeed() const;

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
	Transform m_Transform;


private:
	float m_CurrentModementSpeed;

};


//
// Inline function impleentation
//

inline void APawn::SetMovementSpeed(float Value)
{
	m_MovementSpeed = Value;
}

inline float APawn::GetMovementSpeed() const
{
	return m_MovementSpeed;
}
