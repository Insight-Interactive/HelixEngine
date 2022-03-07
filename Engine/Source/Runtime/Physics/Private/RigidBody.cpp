// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PhysicsPCH.h"

#include "RigidBody.h"


RigidBody::RigidBody()
	: m_IsStatic( false )
	, m_Density( 1.f )
	, m_pRigidActor( nullptr )
	, m_pPhysicsMaterial( nullptr )
	, m_pOwningSceneRef( nullptr )
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::OnCreate()
{
	SetIsStatic( GetIsStatic() );
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
	HE_ASSERT( m_pOwningSceneRef != nullptr ); // Rigid body does not belong to a scene!

	physx::PxShape* pShapes[P_MAX_NUM_ACTOR_SHAPES];
	physx::PxU32 NumShapes = m_pRigidActor->getNbShapes();
	m_pRigidActor->getShapes( pShapes, NumShapes );
	HE_ASSERT( pShapes[0] != nullptr);

	const physx::PxTransform Transform = physx::PxShapeExt::getGlobalPose( *pShapes[0], *m_pRigidActor );
	return FVector3( Transform.p.x, Transform.p.y, Transform.p.z );
}

float RigidBody::GetDensity() const
{
	return m_Density;
}

void RigidBody::SetDensity( const float& NewDensity )
{
	m_Density = NewDensity;
}

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
