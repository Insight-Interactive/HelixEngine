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

	virtual bool IsStatic() const override;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;


private:
	void RegisterCollider( bool StartDisabled );
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