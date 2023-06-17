// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PhysicsPCH.h"

#include "RigidBody.h"


using namespace physx;


HRigidBody::HRigidBody()
	: m_pRigidActor( nullptr )
	, m_pPhysicsMaterial( nullptr )
{
}

HRigidBody::~HRigidBody()
{
}

void HRigidBody::Reset()
{
	if (m_pRigidActor)
	{
		m_pRigidActor->release();
		m_pRigidActor = nullptr;
		m_pPhysicsMaterial->release();
		m_pPhysicsMaterial = nullptr;
	}
}

void HRigidBody::SetGlobalPositionOrientation( const FVector3& NewPosition, const FQuat& NewRotation )
{
	if (!m_pRigidActor)
		return;

	PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
	PxTransform Transform(
		PxVec3( NewPosition.x, NewPosition.y, NewPosition.z ),
		PxQuat( NewRotation.x, NewRotation.y, NewRotation.z, NewRotation.w )
	);
	pDynamic->setGlobalPose( Transform );
}

void HRigidBody::SetSimulatedPosition( const FVector3& NewPosition )
{
	SetGlobalPositionOrientation( NewPosition, GetSimulatedRotation() );
}

void HRigidBody::SetSimulatedRotation( const FQuat& NewRotation )
{
	SetGlobalPositionOrientation( GetSimulatedPosition(), NewRotation );
}

bool HRigidBody::GetIsStatic() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!
	return m_pRigidActor->is<PxRigidDynamic>() != nullptr;
}

FVector3 HRigidBody::GetSimulatedPosition() const
{
	if( !m_pRigidActor) 
		return FVector3::Zero;

	const PxTransform Transform = m_pRigidActor->getGlobalPose();
	return FVector3( Transform.p.x, Transform.p.y, Transform.p.z );
}

FQuat HRigidBody::GetSimulatedRotation() const
{
	if (!m_pRigidActor)
		return FQuat::Identity;
	
	const PxTransform Transform = m_pRigidActor->getGlobalPose();
	return FQuat( Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w );
}

FTransform HRigidBody::GetSimulationWorldTransform()
{
	if (!m_pRigidActor)
		return FTransform();
	const PxTransform Transform = m_pRigidActor->getGlobalPose();
	
	FTransform Return;
	Return.SetPosition( Transform.p.x, Transform.p.y, Transform.p.z );
	Return.SetRotation( FQuat( Transform.q.x, Transform.q.y,Transform.q.z, Transform.q.w ) );
	return Return;
}

float HRigidBody::GetDensity() const
{
	if (PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor ))
	{
		return pDynamic->getMass();
	}
	return 0.f;
}

void HRigidBody::SetDensity( const float& NewDensity )
{
	if (PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor ))
	{
		PxRigidBodyExt::updateMassAndInertia( *pDynamic, NewDensity );
	}
}

void HRigidBody::EnableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	m_pRigidActor->setActorFlag( PxActorFlag::eDISABLE_SIMULATION, false );
}

void HRigidBody::DisableSimulation()
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	m_pRigidActor->setActorFlag( PxActorFlag::eDISABLE_SIMULATION, true );
}

FVector3 HRigidBody::GetLinearVelocity() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	FVector3 Velocity;
	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		PxVec3 Vel = pDynamic->getLinearVelocity();
		Velocity.x = Vel.x; Velocity.y = Vel.y; Velocity.z = Vel.z;
	}

	return Velocity;
}

float HRigidBody::GetAngularDamping() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getAngularDamping();
	}

	return 0.f;
}

void HRigidBody::SetAngularDamping( const float& Damping )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setAngularDamping( Damping );
	}
}

void HRigidBody::SetLinearVelocity( const FVector3& Velocity )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		pDynamic->setLinearVelocity( PxVec3( Velocity.x, Velocity.y, Velocity.z ) );
	}
}

void HRigidBody::AddAcceleration( const FVector3& Acceleration )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( PxVec3( Acceleration.x, Acceleration.y, Acceleration.z ), PxForceMode::eACCELERATION );
	}
}

void HRigidBody::AddForce( const FVector3& Force )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( PxVec3( Force.x, Force.y, Force.z ) );
	}
}

void HRigidBody::AddImpulse( const FVector3& Impulse )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		pDynamic->addForce( PxVec3( Impulse.x, Impulse.y, Impulse.z ), PxForceMode::eIMPULSE );
	}
}

float HRigidBody::GetMass() const
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->getMass();
	}

	return 0.f; // Static objects never move so there's no need to have a mass for them.
}

void HRigidBody::SetMass( const float& NewMass )
{
	HE_ASSERT( m_pRigidActor != nullptr ); // Rigid body has not been initialized!

	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		return pDynamic->setMass( NewMass );
	}
}

bool HRigidBody::GetIsKinematic() const
{
	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( m_pRigidActor );
		const PxRigidBodyFlags Flags = pDynamic->getRigidBodyFlags();
		return Flags.isSet( PxRigidBodyFlag::eKINEMATIC );
	}

	return false;
}

void HRigidBody::SetIsKinematic( bool IsKinematic )
{
	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = m_pRigidActor->is<PxRigidDynamic>();
		pDynamic->setRigidBodyFlag( PxRigidBodyFlag::eKINEMATIC, IsKinematic );
	}
}

bool HRigidBody::IsSleeping() const
{
	if (!GetIsStatic())
	{
		PxRigidDynamic* pDynamic = m_pRigidActor->is<PxRigidDynamic>();
		pDynamic->isSleeping();
	}

	return false;
}

void HRigidBody::SetGravityEnabled( bool Enabled )
{
	if (!m_pRigidActor)
		return;

	m_pRigidActor->setActorFlag( PxActorFlag::eDISABLE_GRAVITY, Enabled );
}

bool HRigidBody::GetIsGravityEnabled() const
{
	PxActorFlags Flags = m_pRigidActor->getActorFlags();
	return !Flags.isSet( PxActorFlag::eDISABLE_GRAVITY );
}

void HRigidBody::ToggleConstrainAxis( EMovementAxis Axis, bool Locked )
{
	if (PxRigidDynamic* pDynamic = m_pRigidActor->is<PxRigidDynamic>())
	{
		switch (Axis)
		{
		case EMovementAxis::MA_X:
			pDynamic->setRigidDynamicLockFlag( PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Locked );
			break;
		case EMovementAxis::MA_Y:
			pDynamic->setRigidDynamicLockFlag( PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Locked );
			break;
		case EMovementAxis::MA_Z:
			pDynamic->setRigidDynamicLockFlag( PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Locked );
			break;
		}
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


/*static*/ const float HPlaneRigidBody::kPlaneDepth = 0.01f;

HPlaneRigidBody::HPlaneRigidBody()
	: m_HalfWidth( 1.f )
	, m_HalfHeight( 1.f )
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
	: m_HalfWidth( 1.f )
	, m_HalfHeight( 1.f )
	, m_HalfDepth( 1.f )
{
}

HCubeRigidBody::~HCubeRigidBody()
{
}


HCapsuleRigidBody::HCapsuleRigidBody()
	: m_Radius( 1.f )
	, m_HalfHeight( 2.f )
{
}

HCapsuleRigidBody::~HCapsuleRigidBody()
{

}