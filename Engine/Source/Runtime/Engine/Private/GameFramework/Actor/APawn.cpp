// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"

#include "Engine/Engine.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "Physics.h"


APawn::APawn( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_Controller(NULL)
{
	m_Controller = AddComponent<HControllerComponent>( "Controller" );

	float mStandingSize = 20;
	float mControllerRadius = 10;
	physx::PxCapsuleControllerDesc cDesc;
	cDesc.material = Physics::CreateDefaultMaterial();
	cDesc.position = physx::PxExtendedVec3(0, 30, 0);
	cDesc.height = mStandingSize;
	cDesc.radius = mControllerRadius;
	cDesc.slopeLimit = 0.0f;
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.02f;
	cDesc.reportCallback = this;
	cDesc.behaviorCallback = this;
	PxController = Physics::CreateCapsuleController( cDesc );
}

APawn::~APawn()
{
	PX_SAFE_RELEASE( PxController );
}

void APawn::Tick( float DeltaTime ) 
{
	Super::Tick( DeltaTime );

}

void APawn::Move(const FVector3& Direction, const float Value)
{
	if (m_RootComponent != nullptr)
	{
		m_Velocity = m_MovementSpeed * Value * GEngine->GetDeltaTime();
		FVector3 Pos = m_RootComponent->GetPosition();
		Pos += Direction * m_Velocity;
		m_RootComponent->SetPosition(Pos);
		/*physx::PxVec3 TargetDisplacement(0);
		TargetDisplacement += physx::PxVec3(Direction.x, Direction.y, Direction.z);
		TargetDisplacement *= 2.5 * Value;
		TargetDisplacement += physx::PxVec3( 0.f, -9.81f, 0.f );
		TargetDisplacement *= GEngine->GetDeltaTime();
		PxController->move( TargetDisplacement, 0.f, GEngine->GetDeltaTime(), physx::PxControllerFilters( 0 ) );*/
		
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

void APawn::onShapeHit( const physx::PxControllerShapeHit& hit )
{
	physx::PxRigidDynamic* actor = hit.shape->getActor()->is<physx::PxRigidDynamic>();
	if (actor)
	{
		// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
		// useless stress on the solver. It would be possible to enable/disable vertical pushes on
		// particular objects, if the gameplay requires it.
		if (hit.dir.y == 0.0f)
		{
			physx::PxReal coeff = actor->getMass() * hit.length;
			physx::PxRigidBodyExt::addForceAtLocalPos( *actor, hit.dir * coeff, physx::PxVec3( 0, 0, 0 ), physx::PxForceMode::eIMPULSE );
		}
	}
}
