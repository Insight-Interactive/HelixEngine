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

	virtual bool IsStatic() const override;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual RigidBody& GetRigidBody() override;
	virtual const RigidBody& GetRigidBody() const override;

private:
	void RegisterCollider( bool StartDisabled );
	void UnRegisterCollider();

private:
	SphereRigidBody m_RigidBody;

};

//
// Inline function implementations
//

FORCEINLINE bool HSphereColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	RigidBody& HSphereColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const RigidBody& HSphereColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
