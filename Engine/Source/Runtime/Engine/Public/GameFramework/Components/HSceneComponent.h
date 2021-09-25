#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Transform.h"


HCOMPONENT()
class HSceneComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HSceneComponent( const HName& Name );
	virtual ~HSceneComponent();

	FVector3 GetPosition() const;
	FVector3 GetRotation() const;
	FVector3 GetScale() const;

	void SetPosition( const FVector3& NewPos );
	void SetRotation( const FVector3& NewRotation );
	void SetScale( const FVector3& NewScale );

protected:
	virtual void Render( ICommandContext& GfxContext ) override;

	virtual void Serialize( rapidjson::Value& Value ) override;
	virtual void Deserialize( const rapidjson::Value& Value ) override;

private:
	FTransform m_Transform;

};

//
// Inline function implementations
//

FORCEINLINE FVector3 HSceneComponent::GetPosition() const
{
	return m_Transform.GetPosition();
}

FORCEINLINE FVector3 HSceneComponent::GetRotation() const
{
	return m_Transform.GetRotation();
}

FORCEINLINE FVector3 HSceneComponent::GetScale() const
{
	return m_Transform.GetScale();
}

FORCEINLINE void HSceneComponent::SetPosition( const FVector3& NewPos )
{
	m_Transform.SetPosition( NewPos );
}

FORCEINLINE void HSceneComponent::SetRotation( const FVector3& NewRotation )
{
	m_Transform.SetRotation( NewRotation );
}

FORCEINLINE void HSceneComponent::SetScale( const FVector3& NewScale )
{
	m_Transform.SetScale( NewScale );
}
