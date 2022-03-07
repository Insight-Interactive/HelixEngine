// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


namespace physx
{
	class PxRigidActor;
	class PxMaterial;
	class PxShape;
	class PxScene;
}

class PHYSICS_API RigidBody
{
	friend class PhysicsScene;
public:
	FVector3 GetSimulatedPosition() const;
	bool GetIsStatic() const;
	void SetIsStatic( bool IsStatic );
	float GetDensity() const;
	void SetDensity( const float& NewDensity );

	physx::PxRigidActor& GetRigidActor();
	physx::PxMaterial& GetPhysicsMaterial();

protected:
	RigidBody();
	virtual ~RigidBody();

	virtual void OnCreate();

	void SetOwningScene( physx::PxScene& OwningScene );

private:
	bool m_IsStatic;
	float m_Density;
	physx::PxRigidActor* m_pRigidActor;
	physx::PxMaterial* m_pPhysicsMaterial;

	physx::PxScene* m_pOwningSceneRef;
};

class PHYSICS_API PlaneRigidBody : public RigidBody
{
public:
	PlaneRigidBody();
	virtual ~PlaneRigidBody();

	float GetWidth() const;
	float GetHeight() const;
	void SetWidth(const float& NewWidth ) ;
	void SetHeight(const float& NewHeight) ;

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

//
// Inline function implementations
//

// RigidBody
//

FORCEINLINE bool RigidBody::GetIsStatic() const
{
	return m_IsStatic;
}

FORCEINLINE void RigidBody::SetOwningScene( physx::PxScene& OwningScene )
{
	m_pOwningSceneRef = &OwningScene;
}

FORCEINLINE	physx::PxRigidActor& RigidBody::GetRigidActor()
{
	return *m_pRigidActor;
}

FORCEINLINE  physx::PxMaterial& RigidBody::GetPhysicsMaterial()
{
	return *m_pPhysicsMaterial;
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
