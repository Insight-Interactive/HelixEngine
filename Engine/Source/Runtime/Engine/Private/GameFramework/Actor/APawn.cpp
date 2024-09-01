// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APawn.h"

#include "Physics.h"
#include "Engine/Engine.h"
#include "GameFramework/Components/HControllerComponent.h"


#define UpdateFootPos()																	\
physx::PxExtendedVec3 FootPos = PxController->getFootPosition();						\
m_RootComponent->SetPosition( (float)FootPos.x, (float)FootPos.y, (float)FootPos.z );	\


APawn::APawn( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_MovementSpeed(kDefaultMovementSpeed)
	, m_SprintSpeed(kDefaultSprintSpeed)
	, m_Velocity(0.f)
	, m_bIsSprinting(false)
	, m_bIsCrouched(false)
	, m_Controller(NULL)
	, m_GravityScale(50.f)
	, Displacement( 0 )
{ 
	m_Controller = AddComponent<HControllerComponent>( "Controller" );

	physx::PxCapsuleControllerDesc cDesc;
	cDesc.material = Physics::CreateDefaultMaterial();
	cDesc.position = physx::PxExtendedVec3( 0, 0, 0 );
	cDesc.height = 43.f;
	cDesc.radius = 30.f; // Height + Radius = 6"1' in inches
	cDesc.slopeLimit = 60.0f;
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

void APawn::FixedUpdate( float Time ) 
{
	Super::FixedUpdate( Time );

	// Move the controller through the world
	Displacement.normalize();
	Displacement.x *= m_MovementSpeed;
	Displacement.y = 0;
	Displacement.y += Physics::Gravity * m_GravityScale;
	Displacement.z *= m_MovementSpeed;
	Displacement *= Time;
	PxController->move( Displacement, 0.f, Time, physx::PxControllerFilters( 0 ) );
	Displacement *= 0;

	// Copy the results
	UpdateFootPos();
}

void APawn::Move(FVector3 Direction, float Value)
{
	Direction *= Value;
	Displacement.x += Direction.x;
	Displacement.y += Direction.y;
	Displacement.z += Direction.z;
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

FVector3 APawn::GetPawnPosition()
{
	physx::PxExtendedVec3 FootPos = PxController->getFootPosition();
	return FVector3( (float)FootPos.x, (float)FootPos.y, (float)FootPos.z );
}

void APawn::Teleport( FVector3 NewPosition )
{
	physx::PxExtendedVec3 NewPos;
	NewPos.x = NewPosition.x;
	NewPos.y = NewPosition.y + ( ( PxController->getHeight() * 0.5f ) + PxController->getRadius() ); // So the foot position is always at <NewPosition>
	NewPos.z = NewPosition.z;
	PxController->setPosition( NewPos );

	UpdateFootPos();
}

float APawn::GetPawnHeight()
{ 
	return PxController->getHeight() + PxController->getRadius();
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
