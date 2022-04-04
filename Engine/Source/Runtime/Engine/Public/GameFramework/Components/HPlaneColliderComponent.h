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

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual RigidBody& GetRigidBody() override;
	virtual const RigidBody& GetRigidBody() const override;

private:
	void RegisterCollider( bool StartDisabled );
	void UnRegisterCollider();

private:
	PlaneRigidBody m_RigidBody;

};


//
// Inline function implementations
//

FORCEINLINE bool HPlaneColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	RigidBody& HPlaneColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const RigidBody& HPlaneColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
