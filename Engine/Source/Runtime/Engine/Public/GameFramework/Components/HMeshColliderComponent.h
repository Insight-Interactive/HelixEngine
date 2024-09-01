// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HColliderComponent.h"


HCOMPONENT()
class HMeshColliderComponent : public HColliderComponent
{
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HMeshColliderComponent );

	virtual bool IsStatic() const override;

protected:

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	HTriangleMeshRigidBody m_RigidBody;

};

//
// Inline function implementations
//

FORCEINLINE bool HMeshColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	HRigidBody& HMeshColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const HRigidBody& HMeshColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}