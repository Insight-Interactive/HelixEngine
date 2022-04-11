// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PhysicsPCH.h"

#include "RigidBody.h"


HRigidBody::HRigidBody()
	: m_IsStatic( false )
	, m_Density( 10.f )
	, m_pRigidActor( nullptr )
	, m_pPhysicsMaterial( nullptr )
{
}

HRigidBody::~HRigidBody()
{
}

void HRigidBody::SetGlobalPositionOrientation( const FVector3& NewPosition, const FQuat& NewRotation )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
	physx::PxTransform Transform(
		physx::PxVec3( NewPosition.x, NewPosition.y, NewPosition.z ),
		physx::PxQuat( NewRotation.x, NewRotation.y, NewRotation.z, NewRotation.w )
	);
	pDynamic->setGlobalPose( Transform );
}

void HRigidBody::SetSimulatedPosition( const FVector3& NewPosition )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	SetGlobalPositionOrientation( NewPosition, GetSimulatedRotation() );
}

void HRigidBody::SetSimulatedRotation( const FQuat& NewRotation )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	SetGlobalPositionOrientation( GetSimulatedPosition(), NewRotation );
}

void HRigidBody::SetIsStatic( bool IsStatic )
{
	m_IsStatic = IsStatic;

	if (m_pRigidActor)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, IsStatic );
}

FVector3 HRigidBody::GetSimulatedPosition() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxShape* pShapes[P_MAX_NUM_ACTOR_SHAPES];
	physx::PxU32 NumShapes = m_pRigidActor->getNbShapes();
	m_pRigidActor->getShapes( pShapes, NumShapes );
	HE_ASSERT( pShapes[0] != nullptr );

	const physx::PxTransform Transform = physx::PxShapeExt::getGlobalPose( *pShapes[0], *m_pRigidActor );
	return FVector3( Transform.p.x, Transform.p.y, Transform.p.z );
}

FQuat HRigidBody::GetSimulatedRotation() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	physx::PxShape* pShapes[P_MAX_NUM_ACTOR_SHAPES];
	physx::PxU32 NumShapes = m_pRigidActor->getNbShapes();
	m_pRigidActor->getShapes( pShapes, NumShapes );
	HE_ASSERT( pShapes[0] != nullptr );

	const physx::PxTransform Transform = physx::PxShapeExt::getGlobalPose( *pShapes[0], *m_pRigidActor );
	return FQuat( Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w );
}

float HRigidBody::GetDensity() const
{
	return m_Density;
}

void HRigidBody::SetDensity( const float& NewDensity )
{
	m_Density = NewDensity;
}

void HRigidBody::EnableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, false );
}

void HRigidBody::DisableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
		m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, true );
}

FVector3 HRigidBody::GetLinearVelocity() const
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

float HRigidBody::GetAngularDamping() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getAngularDamping();
	}

	return 0.f;
}

void HRigidBody::SetAngularDamping( const float& Damping )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setAngularDamping( Damping );
	}
}

void HRigidBody::SetLinearVelocity( const FVector3& Velocity )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setLinearVelocity( physx::PxVec3( Velocity.x, Velocity.y, Velocity.z ) );
	}
}

void HRigidBody::AddAcceleration( const FVector3& Acceleration )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Acceleration.x, Acceleration.y, Acceleration.z ), physx::PxForceMode::eACCELERATION );
	}
}

void HRigidBody::AddForce( const FVector3& Force )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Force.x, Force.y, Force.z ) );
	}
}

void HRigidBody::AddImpulse( const FVector3& Impulse )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( physx::PxVec3( Impulse.x, Impulse.y, Impulse.z ), physx::PxForceMode::eIMPULSE );
	}
}

float HRigidBody::GetMass() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getMass();
	}

	return 0.f;
}

void HRigidBody::SetMass( const float& NewMass )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!m_IsStatic)
	{
		physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->setMass( NewMass );
	}
}


HInfinitePlaneRigidBody::HInfinitePlaneRigidBody()
	: m_Direction( FVector3::Up )
	, m_Distance( 0 )
{

}

HInfinitePlaneRigidBody::~HInfinitePlaneRigidBody()
{

}


/*static*/ const float HPlaneRigidBody::kPlaneHeight = 0.01f;

HPlaneRigidBody::HPlaneRigidBody()
	: m_Width( 1.f )
	, m_Height( 1.f )
{
}

HPlaneRigidBody::~HPlaneRigidBody()
{
}


HSphereRigidBody::HSphereRigidBody()
	: m_Radius( 1.f )
{
}

HSphereRigidBody::~HSphereRigidBody()
{
}


HCubeRigidBody::HCubeRigidBody()
	: m_Width( 1.f )
	, m_Height( 1.f )
	, m_Depth( 1.f )
{
}

HCubeRigidBody::~HCubeRigidBody()
{
}


HCapsuleRigidBody::HCapsuleRigidBody()
{
}

HCapsuleRigidBody::~HCapsuleRigidBody()
{

}