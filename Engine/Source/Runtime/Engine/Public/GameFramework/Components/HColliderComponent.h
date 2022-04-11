#pragma once

#include "GameFramework/Components/HSceneComponent.h"
#include "CollisionHandler.h"

#include "RigidBody.h"


HCOMPONENT()
class HColliderComponent : public HSceneComponent, public PhysicsCallbackHandler
{
	friend class AActor;
	friend class HWorld;
	using Super = HSceneComponent;
public:
	virtual bool IsStatic() const = 0;

	float GetAngularDamping() const;
	FVector3 GetLinearVelocity() const;
	void SetAngularDamping( const float& Damping );
	void SetLinearVelocity( const FVector3& Velocity );

	float GetMass() const;
	void SetMass( const float& NewMass );
	float GetDensity() const;
	void SetDensity( const float& NewDensity );

	void AddAcceleration( const FVector3& Acceleration );
	void AddForce( const FVector3& Force );
	void AddImpulse( const FVector3& Impulse );


protected:
	HE_COMPONENT_GENERATED_BODY( HColliderComponent );

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;
	virtual void OnOwnerDeserializeComplete() override;

	/*
		Called when another collider enters this collider's bounds.
		@param Other - The other collider.
	*/
	virtual void OnEnter( HColliderComponent* Other ) { }
	virtual void OnExit( HColliderComponent* Other ) { }
	virtual void OnStay( HColliderComponent* Other ) { }

protected:
	virtual void OnCreate();
	// Each collier type overrides this to return their rigid body subclass.
	virtual RigidBody& GetRigidBody() = 0;
	virtual const RigidBody& GetRigidBody() const = 0;

	// PhysicsCallbackHandler overrides
	virtual void CollisionEvent( ECollisionType Type, PhysicsCallbackHandler* pCollider ) override;


private:
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
