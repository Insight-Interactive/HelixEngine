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

	void SetRadius( float NewRadius );

protected:
	
	virtual void Tick( float DeltaTime ) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;


private:
	void RegisterCollider();

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
