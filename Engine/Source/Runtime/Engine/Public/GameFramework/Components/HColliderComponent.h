#pragma once

#include "GameFramework/Components/HSceneComponent.h"
#include "CollisionHandler.h"

#include "RigidBody.h"
#include "ModelManager.h"
#include "ConstantBuffer.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/ConstantBufferStructures.h"


HCOMPONENT()
class HColliderComponent : public HSceneComponent, public PhysicsCallbackHandler
{
	friend class AActor;
	friend class HWorld;
	friend class HScene;
	using Super = HSceneComponent;
public:
	virtual bool IsStatic() const = 0;

	bool GetIsTrigger() const;

	float GetAngularDamping() const;
	FVector3 GetLinearVelocity() const;
	void SetAngularDamping( const float& Damping );
	void SetLinearVelocity( const FVector3& Velocity );

	bool GetIsDrawEnabled() const;
	void SetDrawEnabled( const bool& DrawEnabled );
	float GetMass() const;
	void SetMass( const float& NewMass );
	float GetDensity() const;
	void SetDensity( const float& NewDensity );

	void AddAcceleration( const FVector3& Acceleration );
	void AddForce( const FVector3& Force );
	void AddImpulse( const FVector3& Impulse );

	virtual void SetPosition( const FVector3& NewPos ) override;
	virtual void SetRotation( const FQuat& NewRotation ) override;
	virtual void SetScale( const FVector3& NewScale ) override;
	virtual void SetPosition( const float& X, const float& Y, const float& Z ) override;
	virtual void SetRotation( const float& Pitch, const float& Yaw, const float& Roll ) override;
	virtual void SetScale( const float& X, const float& Y, const float& Z ) override;

	virtual void Translate( const float& X, const float& Y, const float& Z );
	virtual void Scale( const float& X, const float& Y, const float& Z );
	
protected:
	HE_COMPONENT_GENERATED_BODY( HColliderComponent );

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void OnOwnerDeserializeComplete() override;
	virtual void Render( FCommandContext& GfxContext ) override;

	/*
		Called when another collider enters this collider's bounds.
		@param Other - The collider entering this collider's bounds.
	*/
	virtual void OnEnter( HColliderComponent* Other ) { }
	/*
		Called when another collider exits this collider's bounds.
		@param Other - The collider exiting this collider's bounds.
	*/
	virtual void OnExit( HColliderComponent* Other ) { }
	/*
		Called when another collider enters and stays in this collider's bounds.
		@param Other - The collider staying in this collider's bounds.
	*/
	virtual void OnStay( HColliderComponent* Other ) { }

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	// Each collier type overrides this to return their rigid body subclass.
	virtual HRigidBody& GetRigidBody() = 0;
	virtual const HRigidBody& GetRigidBody() const = 0;

	// PhysicsCallbackHandler overrides
	virtual void CollisionEvent( ECollisionType Type, PhysicsCallbackHandler* pCollider ) override;

protected:
	// Debug Mesh
	bool									m_CollisionBoundsDrawEnabled;
	HStaticMesh								m_MeshAsset;
	TConstantBuffer<MeshWorldCBData>		m_MeshWorldCB;
	FMaterialInstance*						m_Material;
	// Internal
	bool m_IsStatic;
private:
	/*
		Whether this collider is a trigger or not. If true no physics are applied to this collider.
	*/
	bool m_IsTrigger;

};

//
// Inline function implementations
//

FORCEINLINE void HColliderComponent::CollisionEvent( ECollisionType Type, PhysicsCallbackHandler* pCollider )
{
	switch (Type)
	{
	case CT_Enter:
		OnEnter( DCast<HColliderComponent*>( pCollider ) );
		break;
	case CT_Stay:
		OnStay( DCast<HColliderComponent*>( pCollider ) );
		break;
	case CT_Exit:
		OnExit( DCast<HColliderComponent*>( pCollider ) );
		break;
	default:
		HE_ASSERT( false );
		break;
	}
}

FORCEINLINE bool HColliderComponent::GetIsTrigger() const
{
	return m_IsTrigger;
}

FORCEINLINE float HColliderComponent::GetAngularDamping() const
{
	return GetRigidBody().GetAngularDamping();
}

FORCEINLINE FVector3 HColliderComponent::GetLinearVelocity() const
{
	return GetRigidBody().GetLinearVelocity();
}

FORCEINLINE void HColliderComponent::SetAngularDamping( const float& Damping )
{
	GetRigidBody().SetAngularDamping( Damping );
}

FORCEINLINE void HColliderComponent::SetLinearVelocity( const FVector3& Velocity )
{
	GetRigidBody().SetLinearVelocity( Velocity );
}

FORCEINLINE bool HColliderComponent::GetIsDrawEnabled() const
{
	return m_CollisionBoundsDrawEnabled;
}

FORCEINLINE void HColliderComponent::SetDrawEnabled( const bool& DrawEnabled )
{
	m_CollisionBoundsDrawEnabled = DrawEnabled;
}

FORCEINLINE float HColliderComponent::GetMass() const
{
	return GetRigidBody().GetMass();
}

FORCEINLINE void HColliderComponent::SetMass( const float& NewMass )
{
	GetRigidBody().SetMass( NewMass );
}

FORCEINLINE float HColliderComponent::GetDensity() const
{
	return GetRigidBody().GetDensity();
}

FORCEINLINE void HColliderComponent::SetDensity( const float& NewDensity )
{
	GetRigidBody().SetDensity( NewDensity );
}

FORCEINLINE void HColliderComponent::AddAcceleration( const FVector3& Acceleration )
{
	GetRigidBody().AddAcceleration( Acceleration );
}

FORCEINLINE void HColliderComponent::AddForce( const FVector3& Force )
{
	GetRigidBody().AddForce( Force );
}

FORCEINLINE void HColliderComponent::AddImpulse( const FVector3& Impulse )
{
	GetRigidBody().AddImpulse( Impulse );
}
