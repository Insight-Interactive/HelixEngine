// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Transform.h"


HCOMPONENT()
class HSceneComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HSceneComponent )

	// Return the position in world space not relative to its parent.
	FVector3 GetAbsoluteWorldPosition() const;

	FVector3 GetPosition() const;
	FVector3 GetRotation() const;
	FVector3 GetScale() const;

	FMatrix GetWorldMatrix() const;
	FMatrix GetTranslationMatrix() const;
	FMatrix GetRotationMatrix() const;
	FMatrix GetScaleMatrix() const;

	FVector3 GetLocalForward();
	FVector3 GetLocalBackward();
	FVector3 GetLocalLeft();
	FVector3 GetLocalRight();
	FVector3 GetLocalUp();
	FVector3 GetLocalDown();

	bool GetIsStatic() const;

	void SetPosition( const FVector3& NewPos );
	void SetRotation( const FVector3& NewRotation );
	void SetScale( const FVector3& NewScale );
	void SetPosition( const float& X, const float& Y, const float& Z );
	void SetRotation( const float& X, const float& Y, const float& Z );
	void SetScale( const float& X, const float& Y, const float& Z );

	void Translate( const float& X, const float& Y, const float& Z );
	void Rotate( const float& X, const float& Y, const float& Z );

	void SetIsStatic( bool IsStatic );

	void AttachTo( HSceneComponent* pParent );


protected:
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	virtual void OnPositionChanged() {}
	virtual void OnRotationChanged() {}
	virtual void OnScaleChanged() {}

private:
	HSceneComponent*	m_pParent;
	FTransform			m_Transform;
	bool				m_IsStatic;

};


//
// Inline function implementations
//

FORCEINLINE FVector3 HSceneComponent::GetAbsoluteWorldPosition() const
{
	if (m_pParent)
		return m_pParent->GetAbsoluteWorldPosition() + GetPosition();
	else
		return GetPosition();
}

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

FORCEINLINE FMatrix HSceneComponent::GetWorldMatrix() const
{
	if (m_pParent)
		return m_Transform.GetLocalMatrix() * m_pParent->GetWorldMatrix();
	else
		return m_Transform.GetLocalMatrix();
}

FORCEINLINE FMatrix HSceneComponent::GetTranslationMatrix() const
{
	return m_Transform.GetTranslationMatrix();
}

FORCEINLINE FMatrix HSceneComponent::GetRotationMatrix() const
{
	return m_Transform.GetRotationMatrix();
}

FORCEINLINE FMatrix HSceneComponent::GetScaleMatrix() const
{
	return m_Transform.GetScaleMatrix();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalForward()
{
	return m_Transform.GetLocalForward();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalBackward()
{
	return m_Transform.GetLocalBackward();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalLeft()
{
	return m_Transform.GetLocalLeft();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalRight()
{
	return m_Transform.GetLocalRight();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalUp()
{
	return m_Transform.GetLocalUp();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalDown()
{
	return m_Transform.GetLocalDown();
}

FORCEINLINE bool HSceneComponent::GetIsStatic() const
{
	return m_IsStatic;
}

FORCEINLINE void HSceneComponent::SetPosition( const FVector3& NewPos )
{
	SetPosition( NewPos.x, NewPos.y, NewPos.z );
}

FORCEINLINE void HSceneComponent::SetRotation( const FVector3& NewRotation )
{
	SetRotation( NewRotation.x, NewRotation.y, NewRotation.z );
}

FORCEINLINE void HSceneComponent::SetScale( const FVector3& NewScale )
{
	SetScale( NewScale.x, NewScale.y, NewScale.z );
}

FORCEINLINE void HSceneComponent::SetPosition( const float& X, const float& Y, const float& Z )
{
	m_Transform.SetPosition( X, Y, Z );
	OnPositionChanged();
}

FORCEINLINE void HSceneComponent::SetRotation( const float& X, const float& Y, const float& Z )
{
	m_Transform.SetRotation( X, Y, Z );
	OnRotationChanged();
}

FORCEINLINE void HSceneComponent::SetScale( const float& X, const float& Y, const float& Z )
{
	m_Transform.SetScale( X, Y, Z );
	OnScaleChanged();
}

FORCEINLINE void HSceneComponent::Translate( const float& X, const float& Y, const float& Z )
{
	m_Transform.Translate( X, Y, Z );
}

FORCEINLINE void HSceneComponent::Rotate( const float& X, const float& Y, const float& Z )
{
	m_Transform.Rotate( X, Y, Z );
	OnRotationChanged();
}

FORCEINLINE void HSceneComponent::SetIsStatic( bool IsStatic )
{
	m_IsStatic = IsStatic;
}

FORCEINLINE void HSceneComponent::AttachTo( HSceneComponent* pParent )
{
	if (this == pParent)
		return; // Don't parent ourselves, that doesn't make sense.

	m_pParent = pParent;
}
