#pragma once

#include "GameFramework/Components/HColliderComponent.h"


HCOMPONENT()
class HCubeColliderComponent : public HColliderComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HCubeColliderComponent );

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
	HCubeRigidBody m_RigidBody;

};

//
// Inline function implementations
//

FORCEINLINE bool HCubeColliderComponent::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE	HRigidBody& HCubeColliderComponent::GetRigidBody()
{
	return m_RigidBody;
}

FORCEINLINE	const HRigidBody& HCubeColliderComponent::GetRigidBody() const
{
	return m_RigidBody;
}
