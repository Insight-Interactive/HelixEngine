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
	virtual bool IsStatic() const = 0;

protected:
	HE_COMPONENT_GENERATED_BODY( HColliderComponentInterface );

	virtual void Tick( float DeltaTime ) override;

	virtual void OnEnter( HColliderComponentInterface* Other ) { }
	virtual void OnExit( HColliderComponentInterface* Other ) { }
	virtual void OnStay( HColliderComponentInterface* Other ) { }

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	virtual void OnCreate();
	virtual RigidBody& GetRigidBody() = 0;

};

//
// Inline function implementations
//

