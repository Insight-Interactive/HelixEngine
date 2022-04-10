// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PhysicsPCH.h"

#include "RigidBody.h"


RigidBody::RigidBody()
	: m_IsStatic( false )
	, m_Density( 10.f )
	, m_pRigidActor( nullptr )
	, m_pPhysicsMaterial( nullptr )
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::SetGlobalPositionOrientation( const FVector3& NewPosition, const FQuat& NewRotation )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
	physx::PxTransform Transform(
		physx::PxVec3( NewPosition.x, NewPosition.y, NewPosition.z ),
		physx::PxQuat( NewRotation.x, NewRotation.y, NewRotation.z, NewRotation.w )
	);
	pDynamic->setGlobalPose( Transform );
}

void RigidBody::SetSimulatedPosition( const FVector3& NewPosition )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	SetGlobalPositionOrientation( NewPosition, GetSimulatedRotation() );
}

void RigidBody::SetSimulatedRotation( const FQuat& NewRotation )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	SetGlobalPositionOrientation( GetSimulatedPosition(), NewRotation );
}

void RigidBody::SetIsStatic( bool IsStatic )
{
	m_IsStatic = IsStatic;

	if (m_pRigidActor)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, IsStatic );
}

FVector3 RigidBody::GetSimulatedPosition() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxShape* pShapes[P_MAX_NUM_ACTOR_SHAPES];
	physx::PxU32 NumShapes = m_pRigidActor->getNbShapes();
	m_pRigidActor->getShapes( pShapes, NumShapes );
	HE_ASSERT( pShapes[0] != nullptr );

	const physx::PxTransform Transform = physx::PxShapeExt::getGlobalPose( *pShapes[0], *m_pRigidActor );
	return FVector3( Transform.p.x, Transform.p.y, Transform.p.z );
}

FQuat RigidBody::GetSimulatedRotation() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxShape* pShapes[P_MAX_NUM_ACTOR_SHAPES];
	physx::PxU32 NumShapes = m_pRigidActor->getNbShapes();
	m_pRigidActor->getShapes( pShapes, NumShapes );
	HE_ASSERT( pShapes[0] != nullptr );

	const physx::PxTransform Transform = physx::PxShapeExt::getGlobalPose( *pShapes[0], *m_pRigidActor );
	return FQuat( Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w );
}

float RigidBody::GetDensity() const
{
	return m_Density;
}

void RigidBody::SetDensity( const float& NewDensity )
{
	m_Density = NewDensity;
}

void RigidBody::EnableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, false );
}

void RigidBody::DisableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, true );
}

FVector3 RigidBody::GetLinearVelocity() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	FVector3 Velocity;
	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		physx::PxVec3 Vel = pDynamic->getLinearVelocity();
		Velocity.x = Vel.x; Velocity.y = Vel.y; Velocity.z = Vel.z;
	}

	return Velocity;
}

float RigidBody::GetAngularDamping() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getAngularDamping();
	}

	return 0.f;
}

void RigidBody::SetAngularDamping( const float& Damping )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setAngularDamping( Damping );
	}
}

void RigidBody::SetLinearVelocity( const FVector3& Velocity )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setLinearVelocity( physx::PxVec3( Velocity.x, Velocity.y, Velocity.z ) );
	}
}

void RigidBody::AddAcceleration( const FVector3& Acceleration )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Acceleration.x, Acceleration.y, Acceleration.z ), physx::PxForceMode::eACCELERATION );
	}
}

void RigidBody::AddForce( const FVector3& Force )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Force.x, Force.y, Force.z ) );
	}
}

void RigidBody::AddImpulse( const FVector3& Impulse )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Impulse.x, Impulse.y, Impulse.z ), physx::PxForceMode::eIMPULSE );
	}
}

float RigidBody::GetMass() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getMass();
	}

	return 0.f;
}

void RigidBody::SetMass( const float& NewMass )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->setMass( NewMass );
	}
}


InfinitePlaneRigidBody::InfinitePlaneRigidBody()
	: m_Direction( FVector3::Up )
	, m_Distance( 0 )
{

}

InfinitePlaneRigidBody::~InfinitePlaneRigidBody()
{

}


/*static*/ const float PlaneRigidBody::kPlaneHeight = 0.01f;

PlaneRigidBody::PlaneRigidBody()
	: m_Width( 1.f )
	, m_Height( 1.f )
{
}

PlaneRigidBody::~PlaneRigidBody()
{
}


SphereRigidBody::SphereRigidBody()
	: m_Radius( 1.f )
{
}

SphereRigidBody::~SphereRigidBody()
{
}


CubeRigidBody::CubeRigidBody()
	: m_Width( 1.f )
	, m_Height( 1.f )
	, m_Depth( 1.f )
{
}

CubeRigidBody::~CubeRigidBody()
{
}


CapsuleRigidBody::CapsuleRigidBody()
{
}

CapsuleRigidBody::~CapsuleRigidBody()
{

}