#pragma once

#include "GameFramework/Components/HColliderComponent.h"

#include "ModelManager.h"
#include "ConstantBuffer.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/ConstantBufferStructures.h"


HCOMPONENT()
class HCubeColliderComponent : public HColliderComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HColliderComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HCubeColliderComponent );

	virtual bool IsStatic() const override;

	void SetHalfWidth( float NewHalfWidth );
	void SetHalfHeight( float NewHalfHeight);
	void SetHalfDepth( float NewHalfDepth);
	void SetHalfWidthHeightDepth(float NewHalfWidth, float NewHalfHeight, float NewHalfDepth );

protected:
	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void Tick( float DeltaTime ) override;

	virtual HRigidBody& GetRigidBody() override;
	virtual const HRigidBody& GetRigidBody() const override;

	virtual void OnCreate() override;
	virtual void OnEnter( HColliderComponent* Other ) override;
	virtual void OnExit( HColliderComponent* Other ) override;
	virtual void OnStay( HColliderComponent* Other ) override;

private:
	void RegisterCollider();

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
