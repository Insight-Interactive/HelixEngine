// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


namespace physx
{
	class PxRigidActor;
	class PxMaterial;
}

class RigidBody
{
	friend class PhysicsScene;
public:
	RigidBody();
	virtual ~RigidBody();

	bool GetIsStatic() const;

protected:
	void SetIsStatic( bool IsStatic  );
	physx::PxRigidActor& GetRigidActor();
	physx::PxMaterial& GetPhysicsMaterial();

private:
	bool m_IsStatic;
	physx::PxRigidActor* m_pRigidActor;
	physx::PxMaterial* m_pPhysicsMaterial;

};


//
// Inline function implementations
//

FORCEINLINE bool RigidBody::GetIsStatic() const
{
	return m_IsStatic;
}

FORCEINLINE void RigidBody::SetIsStatic( bool IsStatic )
{
	m_IsStatic = IsStatic;
}

FORCEINLINE	physx::PxRigidActor& RigidBody::GetRigidActor()
{
	return *m_pRigidActor;
}

FORCEINLINE physx::PxMaterial& RigidBody::GetPhysicsMaterial()
{
	return *m_pPhysicsMaterial;
}
