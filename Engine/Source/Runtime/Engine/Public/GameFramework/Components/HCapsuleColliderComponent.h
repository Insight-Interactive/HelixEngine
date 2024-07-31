// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HColliderComponent.h"


HCOMPONENT()
class HCapsuleColliderComponent : public HColliderComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HCapsuleColliderComponent );

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual bool IsStatic() const override;
	float GetHeight() { return GetHalfHeight() + GetRadius(); }
	float GetHalfHeight() { return m_RigidBody.GetHalfHeight(); }
	float GetRadius() { return m_RigidBody.GetRadius(); }
	void SetRadius( float NewRadius );
	void SetHalfHeight( float NewHalfHeight );
	void SetRadiusAndHalfHeight( float NewRadius, float NewHalfHeight );

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	void RegisterCollider();
	void UnRegisterCollider();

private:
	HCapsuleRigidBody m_RigidBody;

};

//
// Inline function implementations
//

FORCEINLINE bool HCapsuleColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	HRigidBody& HCapsuleColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const HRigidBody& HCapsuleColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
