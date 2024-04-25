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

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual bool IsStatic() const override;
	void SetHalfWidth( float NewHalfWidth );
	void SetHalfHeight( float NewHalfHeight);
	void SetHalfDepth( float NewHalfDepth);
	void SetHalfWidthHeightDepth(float NewHalfWidth, float NewHalfHeight, float NewHalfDepth );

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

	virtual void OnEnter( HColliderComponent* Other ) override;
	virtual void OnExit( HColliderComponent* Other ) override;
	virtual void OnStay( HColliderComponent* Other ) override;

private:
	void RegisterCollider();
	void UnregisterCollider();

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
