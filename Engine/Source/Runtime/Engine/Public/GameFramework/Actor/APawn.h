// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"
#include "Transform.h"
#include "characterkinematic/PxController.h" // for PxUserControllerHitReport
#include "characterkinematic/PxControllerBehavior.h"

struct FJump
{
	const float JumpGravity = -500.f;

	FJump()
		: CurrentHeight( 0.01f )
		, MaxHeight( 0.f )
		, JumpTime( 0.f )
		, Jumping( false )
	{
	}
	void StartJump( float JumpHeight )
	{
		if (Jumping)
			return;

		JumpTime = 0.f;
		MaxHeight = JumpHeight;
		Jumping = true;
	}
	void StopJump()
	{
		if (!Jumping)
			return;

		Jumping = false;
	}
	float GetHeight(float DeltaTime, float GravityScale = 1.f )
	{
		if (!Jumping)
			return 0.f;

		//if (CurrentHeight >= MaxHeight)
		//	return 0.f;
		//else
		//	CurrentHeight = (CurrentHeight + DeltaTime) * 3.f;

		JumpTime += DeltaTime;
		const float Height = JumpGravity * JumpTime * JumpTime + MaxHeight * JumpTime;
		return (Height * DeltaTime) * GravityScale;
	}
	float CurrentHeight;

	float MaxHeight;
	float JumpTime;
	bool Jumping;
};

namespace physx
{
	class PxControllerManager;
	class PxCapsuleController;
	class PxShape;
}
class HControllerComponent;


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
	virtual void Tick( float Time ) override;

	float GetPawnHeight();
	FVector3 GetPawnPosition();
	void Teleport( FVector3 NewPosition );

	void Sprint();
	void Jump();
	void SetMovementSpeed(float Value);
	float GetMovementSpeed() const;

protected:
	virtual void SetupController(HControllerComponent& Controller) { /* Override for behavior. */ }
	void Move(FVector3 Direction, float Value);

protected:
	virtual void							onShapeHit( const physx::PxControllerShapeHit& hit );
	virtual void							onControllerHit( const physx::PxControllersHit& hit ) {}
	virtual void							onObstacleHit( const physx::PxControllerObstacleHit& hit ) {}
	
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxShape&, const physx::PxActor& ) { return physx::PxControllerBehaviorFlags( 0 ); }
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxController& ) { return physx::PxControllerBehaviorFlags( 0 ); }
	virtual physx::PxControllerBehaviorFlags		getBehaviorFlags( const physx::PxObstacle& ) { return physx::PxControllerBehaviorFlags( 0 ); }

protected:

	float m_GravityScale;
	float m_MovementSpeed;
	float m_SprintSpeed;
	bool m_bIsSprinting;
	bool m_bIsCrouched;
	FJump m_Jump;

	HControllerComponent* m_Controller;
	
	physx::PxCapsuleController* PxController;
	physx::PxVec3 Displacement;

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
