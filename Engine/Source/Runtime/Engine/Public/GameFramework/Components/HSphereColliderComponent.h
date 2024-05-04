// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HColliderComponent.h"


HCOMPONENT()
class HSphereColliderComponent : public HColliderComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HSphereColliderComponent );

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual bool IsStatic() const override;
	void SetRadius( float NewRadius );

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	void RegisterCollider();
	void UnRegisterCollider();

private:
	HSphereRigidBody m_RigidBody;

};

//
// Inline function implementations
//

FORCEINLINE bool HSphereColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	HRigidBody& HSphereColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const HRigidBody& HSphereColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
