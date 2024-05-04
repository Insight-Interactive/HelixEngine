// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HColliderComponent.h"


HCOMPONENT()
class HPlaneColliderComponent : public HColliderComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HPlaneColliderComponent );

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual bool IsStatic() const override;
	void SetWidth( float Width );
	void SetHeight( float Depth );
	void SetWidthAndHeight( float Width, float Depth );

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	void RegisterCollider();
	void UnRegisterCollider();

private:
	HPlaneRigidBody m_RigidBody;

};


//
// Inline function implementations
//

FORCEINLINE bool HPlaneColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	HRigidBody& HPlaneColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const HRigidBody& HPlaneColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
