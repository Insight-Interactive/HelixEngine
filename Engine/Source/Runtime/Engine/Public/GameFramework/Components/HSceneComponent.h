// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Transform.h"


HCOMPONENT()
class HSceneComponent : public HActorComponent
{
	friend class AActor;
	friend class HWorld; // For transform serialization
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HSceneComponent )

	// Return the position in world space not relative to its parent.
	FVector3 GetWorldPosition() const;

	FVector3 GetPosition() const;
	FQuat GetWorldRotation() const;
	FQuat GetRotation() const;
	FVector3 GetEulerRotation() const;
	FVector3 GetScale() const;

	FMatrix GetLocalMatrix() const;
	FMatrix GetWorldMatrix() const;
	FMatrix GetTranslationMatrix() const;
	FMatrix GetRotationMatrix() const;
	FMatrix GetScaleMatrix() const;

	FVector3 GetLocalForward() const;
	FVector3 GetLocalBackward() const;
	FVector3 GetLocalLeft() const;
	FVector3 GetLocalRight() const;
	FVector3 GetLocalUp() const;
	FVector3 GetLocalDown() const;

	bool GetIsStatic() const;

	void LookAt( const FVector3& Pos );

	virtual void SetPosition( const FVector3& NewPos );
	virtual void SetRotation( const FQuat& NewRotation );
	virtual void SetRotation( const FVector3& NewEulerRotation );
	virtual void SetScale( const FVector3& NewScale );
	virtual void SetPosition( const float& X, const float& Y, const float& Z );
	virtual void SetRotation( const float& Pitch, const float& Yaw, const float& Roll );
	virtual void SetScale( const float& X, const float& Y, const float& Z );

	virtual void Translate( const float& X, const float& Y, const float& Z );
	virtual void Rotate( const float& Pitch, const float& Yaw, const float& Roll );
	virtual void Scale( const float& X, const float& Y, const float& Z );

	void SetIsStatic( bool IsStatic );

	void AttachTo( HSceneComponent* pParent );


protected:
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

	HSceneComponent*	m_pParent;
	FTransform			m_Transform;
	bool				m_IsStatic;

};


//
// Inline function implementations
//

FORCEINLINE FVector3 HSceneComponent::GetWorldPosition() const
{
	if (m_pParent)
		return m_pParent->GetWorldPosition() + GetPosition();
	else
		return GetPosition();
}

FORCEINLINE FVector3 HSceneComponent::GetPosition() const
{
	return m_Transform.GetPosition();
}

FORCEINLINE FQuat HSceneComponent::GetWorldRotation() const
{
	FVector3 Position;
	FVector3 Scale;
	FQuat Rotation;
	GetWorldMatrix().Decompose( Scale, Rotation, Position );

	return Rotation;
}

FORCEINLINE FQuat HSceneComponent::GetRotation() const
{
	return m_Transform.GetRotation();
}

FORCEINLINE FVector3 HSceneComponent::GetEulerRotation() const
{
	return m_Transform.GetEulerRotation();
}

FORCEINLINE FVector3 HSceneComponent::GetScale() const
{
	return m_Transform.GetScale();
}

FORCEINLINE FMatrix HSceneComponent::GetLocalMatrix() const
{
	return m_Transform.GetLocalMatrix();
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

FORCEINLINE FVector3 HSceneComponent::GetLocalForward() const
{
	return m_Transform.GetLocalForward();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalBackward() const
{
	return m_Transform.GetLocalBackward();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalLeft() const
{
	return m_Transform.GetLocalLeft();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalRight() const
{
	return m_Transform.GetLocalRight();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalUp() const
{
	return m_Transform.GetLocalUp();
}

FORCEINLINE FVector3 HSceneComponent::GetLocalDown() const
{
	return m_Transform.GetLocalDown();
}

FORCEINLINE bool HSceneComponent::GetIsStatic() const
{
	return m_IsStatic;
}

FORCEINLINE void HSceneComponent::LookAt( const FVector3& Pos )
{
	m_Transform.LookAt( Pos );
}

FORCEINLINE void HSceneComponent::SetPosition( const FVector3& NewPos )
{
	m_Transform.SetPosition( NewPos.x, NewPos.y, NewPos.z );
}

FORCEINLINE void HSceneComponent::SetRotation( const FQuat& NewRotation )
{
	m_Transform.SetRotation( NewRotation );
}

FORCEINLINE void HSceneComponent::SetRotation( const FVector3& NewEulerRotation )
{
	SetRotation( FQuat::CreateFromYawPitchRoll( NewEulerRotation.y, NewEulerRotation.x, NewEulerRotation.z ) );
}

FORCEINLINE void HSceneComponent::SetScale( const FVector3& NewScale )
{
	m_Transform.SetScale( NewScale.x, NewScale.y, NewScale.z );
}

FORCEINLINE void HSceneComponent::SetPosition( const float& X, const float& Y, const float& Z )
{
	m_Transform.SetPosition( X, Y, Z );
}

FORCEINLINE void HSceneComponent::SetRotation( const float& Pitch, const float& Yaw, const float& Roll )
{
	m_Transform.SetRotation( Pitch, Yaw, Roll );
}

FORCEINLINE void HSceneComponent::SetScale( const float& X, const float& Y, const float& Z )
{
	m_Transform.SetScale( X, Y, Z );
}

FORCEINLINE void HSceneComponent::Translate( const float& X, const float& Y, const float& Z )
{
	m_Transform.Translate( X, Y, Z );
}

FORCEINLINE void HSceneComponent::Rotate( const float& Pitch, const float& Yaw, const float& Roll )
{
	m_Transform.Rotate( Pitch, Yaw, Roll );
}

FORCEINLINE void HSceneComponent::Scale( const float& X, const float& Y, const float& Z )
{
	m_Transform.Scale( X, Y, Z );
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
