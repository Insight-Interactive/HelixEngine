// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"
#include "Transform.h"
#include "characterkinematic/PxController.h" // for PxUserControllerHitReport
#include "characterkinematic/PxControllerBehavior.h"

namespace physx
{
	class PxControllerManager;
	class PxCapsuleController;
	class PxShape;
}
class HControllerComponent;

static const float kDefaultMovementSpeed = 32.f;
static const float kDefaultSprintSpeed = kDefaultMovementSpeed * 3.f;

HCLASS()
class APawn : public AActor, public physx::PxUserControllerHitReport, public physx::PxControllerBehaviorCallback
{
	using Super = AActor;
public:
	HE_GENERATED_BODY( APawn )

		 
	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		SetupController(*m_Controller);
	}

	void Tick( float DeltaTime ) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Sprint();

	void SetMovementSpeed(float Value);
	float GetMovementSpeed() const;

protected:
	virtual void SetupController(HControllerComponent& Controller) { /* Override for behavior. */ }


protected:
	void Move(const FVector3& Direction, const float Value);

protected:
	virtual void							onShapeHit( const physx::PxControllerShapeHit& hit );
	virtual void							onControllerHit( const physx::PxControllersHit& hit ) {}
	virtual void							onObstacleHit( const physx::PxControllerObstacleHit& hit ) {}
	
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxShape&, const physx::PxActor& ) { return physx::PxControllerBehaviorFlags( 0 ); }
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxController& ) { return physx::PxControllerBehaviorFlags( 0 ); }
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxObstacle& ) { return physx::PxControllerBehaviorFlags( 0 ); }

protected:
	float m_MovementSpeed;
	float m_SprintSpeed;
	float m_Velocity;
	bool m_bIsSprinting;

	HControllerComponent* m_Controller;
	
	physx::PxCapsuleController* PxController;

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
