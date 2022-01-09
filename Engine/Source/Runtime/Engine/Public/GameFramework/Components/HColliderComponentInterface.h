#pragma once

#include "GameFramework/Components/HSceneComponent.h"

#include "RigidBody.h"


HCOMPONENT()
class HColliderComponentInterface : public HSceneComponent
{
	friend class AActor;
	friend class HWorld;
	using Super = HSceneComponent;
public:

	bool IsStatic() const;

protected:
	HE_COMPONENT_GENERATED_BODY( HColliderComponentInterface );

	virtual void OnObjectEnter( HColliderComponentInterface* Other ) { }
	virtual void OnObjectExit( HColliderComponentInterface* Other ) { }
	virtual void OnObjectStay( HColliderComponentInterface* Other ) { }

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	virtual void OnCreate();
	RigidBody& GetRigidBody();

private:
	RigidBody m_RigidBody;
	bool m_DefaultDontUse;

};

//
// Inline function implementations
//

FORCEINLINE bool HColliderComponentInterface::IsStatic() const
{
	return m_RigidBody.GetIsStatic();
}

FORCEINLINE RigidBody& HColliderComponentInterface::GetRigidBody()
{
	return m_RigidBody;
}
