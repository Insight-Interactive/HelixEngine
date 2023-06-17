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

	virtual bool IsStatic() const override;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Tick( float DeltaTime );

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

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
