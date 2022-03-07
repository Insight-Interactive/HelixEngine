#pragma once

#include "GameFramework/Components/HColliderComponentInterface.h"


HCOMPONENT()
class HSphereColliderComponent : public HColliderComponentInterface
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponentInterface;
public:
	HE_COMPONENT_GENERATED_BODY( HSphereColliderComponent );

	virtual bool IsStatic() const override;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual RigidBody& GetRigidBody() override;


private:
	void RegisterCollider();
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

