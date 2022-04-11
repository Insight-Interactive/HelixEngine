// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


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
	void SetGlobalPositionOrientation( const FVector3& NewPosition, const FQuat& NewRotation );
	void SetSimulatedPosition( const FVector3& NewPosition );
	void SetSimulatedRotation( const FQuat& NewRotation );
	FVector3 GetSimulatedPosition() const;
	FQuat GetSimulatedRotation() const;
	bool GetIsStatic() const;
	void SetIsStatic( bool IsStatic );
	float GetDensity() const;
	void SetDensity( const float& NewDensity );
	void EnableSimulation();
	void DisableSimulation();
	float GetMass() const;
	void SetMass( const float& NewMass );

	FVector3 GetLinearVelocity() const;
	float GetAngularDamping() const;
	void SetAngularDamping( const float& Damping );
	void SetLinearVelocity( const FVector3& Velocity );
	void AddAcceleration( const FVector3& Acceleration );
	void AddForce( const FVector3& Force );
	void AddImpulse( const FVector3& Impulse );

	physx::PxRigidActor& GetRigidActor();
	physx::PxMaterial& GetPhysicsMaterial();


protected:
	HRigidBody();
	virtual ~HRigidBody();

private:
	bool m_IsStatic;
	float m_Density;
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
	static const float kPlaneHeight;
public:
	HPlaneRigidBody();
	virtual ~HPlaneRigidBody();

	float GetWidth() const;
	float GetHeight() const;
	void SetWidth( const float& NewWidth );
	void SetHeight( const float& NewHeight );

private:
	float m_Width;
	float m_Height;
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

	float GetWidth() const;
	float GetHeight() const;
	float GetDepth() const;

	void SetWidth( const float& NewWidth );
	void SetHeight( const float& NewHeight );
	void SetDepth( const float& NewDepth );

private:
	float m_Width;
	float m_Height;
	float m_Depth;

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
	float GetLength() const;

	void SetRadius( const float& NewRadius );
	void SetLength( const float& NewLength );

private:
	float m_Radius;
	float m_Length;
};


//
// Inline function implementations
//

// RigidBody
//

FORCEINLINE bool HRigidBody::GetIsStatic() const
{
	return m_IsStatic;
}

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

FORCEINLINE float HPlaneRigidBody::GetWidth() const
{
	return m_Width;
}

FORCEINLINE float HPlaneRigidBody::GetHeight() const
{
	return m_Height;
}

FORCEINLINE void HPlaneRigidBody::SetWidth( const float& NewWidth )
{
	m_Width = NewWidth;
}

FORCEINLINE void HPlaneRigidBody::SetHeight( const float& NewHeight )
{
	m_Height = NewHeight;
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

FORCEINLINE float HCubeRigidBody::GetWidth() const
{
	return m_Width;
}

FORCEINLINE float HCubeRigidBody::GetHeight() const
{
	return m_Height;
}

FORCEINLINE float HCubeRigidBody::GetDepth() const
{
	return m_Depth;
}

FORCEINLINE void HCubeRigidBody::SetWidth( const float& NewWidth )
{
	m_Width = NewWidth;
}

FORCEINLINE void HCubeRigidBody::SetHeight( const float& NewHeight )
{
	m_Height = NewHeight;
}

FORCEINLINE void HCubeRigidBody::SetDepth( const float& NewDepth )
{
	m_Depth = NewDepth;
}

// Capsule
//

FORCEINLINE	float HCapsuleRigidBody::GetRadius() const
{
	return m_Radius;
}
FORCEINLINE	float HCapsuleRigidBody::GetLength() const
{
	return m_Length;
}

FORCEINLINE void HCapsuleRigidBody::SetRadius( const float& NewRadius )
{
	m_Radius = NewRadius;
}

FORCEINLINE void HCapsuleRigidBody::SetLength( const float& NewLength )
{
	m_Length = NewLength;
}
