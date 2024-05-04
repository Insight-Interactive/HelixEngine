// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Transform.h"


namespace physx
{
	class PxRigidActor;
	class PxMaterial;
	class PxShape;
}

/*
	Base rigid body class in which all rigid body types derived from.
*/
class PHYSICS_API HRigidBody
{
	friend class HPhysicsScene;
public:
	enum EMovementAxis
	{
		MA_X,
		MA_Y,
		MA_Z,
	};
public:
	void SetGlobalPositionOrientation( const FVector3& NewPosition, const FQuat& NewRotation );
	void SetSimulatedPosition( const FVector3& NewPosition );
	void SetSimulatedRotation( const FQuat& NewRotation );
	FVector3 GetSimulatedPosition() const;
	FQuat GetSimulatedRotation() const;
	bool GetIsStatic() const;
	float GetDensity() const;
	void SetDensity( const float& NewDensity );
	void EnableSimulation();
	void DisableSimulation();
	bool IsSimulationEnabled();
	float GetMass() const;
	void SetMass( const float& NewMass );
	bool GetIsKinematic() const;
	void SetIsKinematic( bool IsKinematic );

	bool IsSleeping() const;
	void SetGravityEnabled( bool Enabled );
	bool GetIsGravityEnabled() const;

	void ToggleConstrainAxis( EMovementAxis Axis, bool Locked );

	FTransform GetSimulationWorldTransform();

	FVector3 GetLinearVelocity() const;
	float GetAngularDamping() const;
	void SetAngularDamping( const float& Damping );
	void SetLinearVelocity( const FVector3& Velocity );
	void AddAcceleration( const FVector3& Acceleration );
	void AddForce( const FVector3& Force );
	void AddImpulse( const FVector3& Impulse );

	physx::PxRigidActor& GetRigidActor();
	physx::PxMaterial& GetPhysicsMaterial();

	bool IsValid() const;
	void Reset();

protected:
	HRigidBody();
	virtual ~HRigidBody();

protected:
	physx::PxRigidActor* m_pRigidActor;
	physx::PxMaterial* m_pPhysicsMaterial;

};

/*
	A plane with infinite width and height that points in a single direction.
*/
class PHYSICS_API HInfinitePlaneRigidBody : public HRigidBody
{
public:
	HInfinitePlaneRigidBody();
	virtual ~HInfinitePlaneRigidBody();

	float GetDistance() const;
	void SetDistance( const float& NewDistance);
	FVector3 GetDirection() const;
	void SetDirection( const FVector3& NewDirection );

private:
	FVector3 m_Direction;
	float m_Distance;

};

/*
	A plane with a defined width and height.	
*/
class PHYSICS_API HPlaneRigidBody : public HRigidBody
{
public:
	static const float kPlaneDepth;
public:
	HPlaneRigidBody();
	virtual ~HPlaneRigidBody();

	static float GetConstantDepth();
	float GetHalfWidth() const;
	float GetHalfHeight() const;
	void SetHalfWidth( const float& NewHalfWidth );
	void SetHalfHeight( const float& NewHalfHeight );

private:
	float m_HalfWidth;
	float m_HalfHeight;
};

/*
	A sphere with a radius.
*/
class PHYSICS_API HSphereRigidBody : public HRigidBody
{
public:
	HSphereRigidBody();
	virtual ~HSphereRigidBody();

	float GetRadius() const;
	void SetRadius( const float& NewRadius );

private:
	float m_Radius;

};

/*
	A cube with width, height, and depth.
*/
class PHYSICS_API HCubeRigidBody : public HRigidBody
{
public:
	HCubeRigidBody();
	virtual ~HCubeRigidBody();

	float GetHalfWidth() const;
	float GetHalfHeight() const;
	float GetHalfDepth() const;

	void SetHalfWidth( const float& NewHalfWidth );
	void SetHalfHeight( const float& NewHalfHeight );
	void SetHalfDepth( const float& NewHalfDepth );

private:
	float m_HalfWidth;
	float m_HalfHeight;
	float m_HalfDepth;

};

/*
	A capsule with a radius and length;
*/
class PHYSICS_API HCapsuleRigidBody : public HRigidBody
{
public:
	HCapsuleRigidBody();
	virtual ~HCapsuleRigidBody();

	float GetRadius() const;
	float GetHalfHeight() const;

	void SetRadius( const float& NewRadius );
	void SetHalfHeight( const float& NewHalfHeight );

private:
	float m_Radius;
	float m_HalfHeight;
};


//
// Inline function implementations
//

// RigidBody
//

FORCEINLINE	physx::PxRigidActor& HRigidBody::GetRigidActor()
{
	HE_ASSERT( m_pRigidActor != nullptr );
	return *m_pRigidActor;
}

FORCEINLINE physx::PxMaterial& HRigidBody::GetPhysicsMaterial()
{
	HE_ASSERT( m_pPhysicsMaterial != nullptr );
	return *m_pPhysicsMaterial;
}

FORCEINLINE bool HRigidBody::IsValid() const
{
	return m_pRigidActor != nullptr && m_pPhysicsMaterial != nullptr;
}


// Infinite Plane
//

FORCEINLINE float HInfinitePlaneRigidBody::GetDistance() const
{
	return m_Distance;
}

FORCEINLINE void HInfinitePlaneRigidBody::SetDistance( const float& NewDistance )
{
	m_Distance = NewDistance;
}

FORCEINLINE FVector3 HInfinitePlaneRigidBody::GetDirection() const
{
	return m_Direction;
}

FORCEINLINE void HInfinitePlaneRigidBody::SetDirection( const FVector3& NewDirection )
{
	m_Direction = NewDirection;
}


// Plane
//

/*static*/ FORCEINLINE float HPlaneRigidBody::GetConstantDepth()
{
	return kPlaneDepth;
}

FORCEINLINE float HPlaneRigidBody::GetHalfWidth() const
{
	return m_HalfWidth;
}

FORCEINLINE float HPlaneRigidBody::GetHalfHeight() const
{
	return m_HalfHeight;
}

FORCEINLINE void HPlaneRigidBody::SetHalfWidth( const float& NewHalfWidth )
{
	m_HalfWidth = NewHalfWidth;
}

FORCEINLINE void HPlaneRigidBody::SetHalfHeight( const float& NewHalfHeight )
{
	m_HalfHeight = NewHalfHeight;
}

// Sphere
//

FORCEINLINE float HSphereRigidBody::GetRadius() const
{
	return m_Radius;
}

FORCEINLINE void HSphereRigidBody::SetRadius( const float& NewRadius )
{
	m_Radius = NewRadius;
}

// Cube
//

FORCEINLINE float HCubeRigidBody::GetHalfWidth() const
{
	return m_HalfWidth;
}

FORCEINLINE float HCubeRigidBody::GetHalfHeight() const
{
	return m_HalfHeight;
}

FORCEINLINE float HCubeRigidBody::GetHalfDepth() const
{
	return m_HalfDepth;
}

FORCEINLINE void HCubeRigidBody::SetHalfWidth( const float& NewHalfWidth )
{
	m_HalfWidth = NewHalfWidth;
}

FORCEINLINE void HCubeRigidBody::SetHalfHeight( const float& NewHalfHeight )
{
	m_HalfHeight = NewHalfHeight;
}

FORCEINLINE void HCubeRigidBody::SetHalfDepth( const float& NewHalfDepth )
{
	m_HalfDepth = NewHalfDepth;
}

// Capsule
//

FORCEINLINE	float HCapsuleRigidBody::GetRadius() const
{
	return m_Radius;
}
FORCEINLINE	float HCapsuleRigidBody::GetHalfHeight() const
{
	return m_HalfHeight;
}

FORCEINLINE void HCapsuleRigidBody::SetRadius( const float& NewRadius )
{
	m_Radius = NewRadius;
}

FORCEINLINE void HCapsuleRigidBody::SetHalfHeight( const float& NewHalfHeight )
{
	m_HalfHeight = NewHalfHeight;
}
