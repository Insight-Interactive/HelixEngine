// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


namespace physx
{
	class PxRigidActor;
	class PxMaterial;
	class PxShape;
}

class PHYSICS_API RigidBody
{
	friend class PhysicsScene;
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
	RigidBody();
	virtual ~RigidBody();

private:
	bool m_IsStatic;
	float m_Density;
	physx::PxRigidActor* m_pRigidActor;
	physx::PxMaterial* m_pPhysicsMaterial;

};


class PHYSICS_API InfinitePlaneRigidBody : public RigidBody
{
public:
	InfinitePlaneRigidBody();
	virtual ~InfinitePlaneRigidBody();

	float GetDistance() const;
	void SetDistance( const float& NewDistance);
	FVector3 GetDirection() const;
	void SetDirection( const FVector3& NewDirection );

private:
	FVector3 m_Direction;
	float m_Distance;

};

class PHYSICS_API PlaneRigidBody : public RigidBody
{
public:
	static const float kPlaneHeight;
public:
	PlaneRigidBody();
	virtual ~PlaneRigidBody();

	float GetWidth() const;
	float GetHeight() const;
	void SetWidth( const float& NewWidth );
	void SetHeight( const float& NewHeight );

private:
	float m_Width;
	float m_Height;
};

class PHYSICS_API SphereRigidBody : public RigidBody
{
public:
	SphereRigidBody();
	virtual ~SphereRigidBody();

	float GetRadius() const;
	void SetRadius( const float& NewRadius );

private:
	float m_Radius;

};

class PHYSICS_API CubeRigidBody : public RigidBody
{
public:
	CubeRigidBody();
	virtual ~CubeRigidBody();

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

class PHYSICS_API CapsuleRigidBody : public RigidBody
{
public:
	CapsuleRigidBody();
	virtual ~CapsuleRigidBody();

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

FORCEINLINE bool RigidBody::GetIsStatic() const
{
	return m_IsStatic;
}

FORCEINLINE	physx::PxRigidActor& RigidBody::GetRigidActor()
{
	return *m_pRigidActor;
}

FORCEINLINE physx::PxMaterial& RigidBody::GetPhysicsMaterial()
{
	return *m_pPhysicsMaterial;
}


// Infinite Plane
//

FORCEINLINE float InfinitePlaneRigidBody::GetDistance() const
{
	return m_Distance;
}

FORCEINLINE void InfinitePlaneRigidBody::SetDistance( const float& NewDistance )
{
	m_Distance = NewDistance;
}

FORCEINLINE FVector3 InfinitePlaneRigidBody::GetDirection() const
{
	return m_Direction;
}

FORCEINLINE void InfinitePlaneRigidBody::SetDirection( const FVector3& NewDirection )
{
	m_Direction = NewDirection;
}


// Plane
//

FORCEINLINE float PlaneRigidBody::GetWidth() const
{
	return m_Width;
}

FORCEINLINE float PlaneRigidBody::GetHeight() const
{
	return m_Height;
}

FORCEINLINE void PlaneRigidBody::SetWidth( const float& NewWidth )
{
	m_Width = NewWidth;
}

FORCEINLINE void PlaneRigidBody::SetHeight( const float& NewHeight )
{
	m_Height = NewHeight;
}

// Sphere
//

FORCEINLINE float SphereRigidBody::GetRadius() const
{
	return m_Radius;
}

FORCEINLINE void SphereRigidBody::SetRadius( const float& NewRadius )
{
	m_Radius = NewRadius;
}

// Cube
//

FORCEINLINE float CubeRigidBody::GetWidth() const
{
	return m_Width;
}

FORCEINLINE float CubeRigidBody::GetHeight() const
{
	return m_Height;
}

FORCEINLINE float CubeRigidBody::GetDepth() const
{
	return m_Depth;
}

FORCEINLINE void CubeRigidBody::SetWidth( const float& NewWidth )
{
	m_Width = NewWidth;
}

FORCEINLINE void CubeRigidBody::SetHeight( const float& NewHeight )
{
	m_Height = NewHeight;
}

FORCEINLINE void CubeRigidBody::SetDepth( const float& NewDepth )
{
	m_Depth = NewDepth;
}

// Capsule
//

FORCEINLINE	float CapsuleRigidBody::GetRadius() const
{
	return m_Radius;
}
FORCEINLINE	float CapsuleRigidBody::GetLength() const
{
	return m_Length;
}

FORCEINLINE void CapsuleRigidBody::SetRadius( const float& NewRadius )
{
	m_Radius = NewRadius;
}

FORCEINLINE void CapsuleRigidBody::SetLength( const float& NewLength )
{
	m_Length = NewLength;
}
