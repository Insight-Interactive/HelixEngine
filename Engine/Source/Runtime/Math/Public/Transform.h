// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "MathFwd.h"
#include "CoreFwd.h"

#include "MathCore.h"

/*
	Represents 3D transformations in the world.
*/
class MATH_API FTransform
{
public:
	FTransform();
	~FTransform();
	FTransform( FTransform&& Other ) noexcept;
	FTransform( const FTransform& FTransform );
	FTransform& operator = ( const FTransform& Other );

	FVector3 GetPosition()		const { return m_Position; }
	FQuat GetRotation()			const {	return m_Rotation; }
	FAngles GetEulerRotation() const { return m_Rotation.ToEulerAngles(); }
	FVector3 GetScale()			const { return m_Scale; }

	FMatrix GetWorldMatrix()
	{
		if (m_Parent)
			return GetLocalMatrix() * m_Parent->GetWorldMatrix();
		else
			return GetLocalMatrix();
	}

	FVector3 GetWorldPosition()
	{
		FVector3 Position;
		FVector3 Scale;
		FQuat Rotation;
		GetWorldMatrix().Decompose( Scale, Rotation, Position );
		return Position;
	}

	FQuat GetWorldScale()
	{
		FVector3 Position;
		FVector3 Scale;
		FQuat Rotation;
		GetWorldMatrix().Decompose( Scale, Rotation, Position );
		return Rotation;
	}

	void LinkTo( FTransform& Parent, FVector3 Offset = FVector3::Zero ) 
	{ 
		m_Parent = &Parent;
		Translate( Offset.x, Offset.y, Offset.z );
	}
	void Unlink() { m_Parent = nullptr; }

	void SetPosition( const float& X, const float& Y, const float& Z );
	void SetRotation( const float& Pitch, const float& Yaw, const float& Roll );
	void SetScale( const float& X, const float& Y, const float& Z );

	void SetPosition( const FVector3& Position );
	void SetRotation( const FAngles& Euler );
	void SetRotation( const FQuat& Rotation );
	void SetScale( const FVector3& Scale );

	void Translate( const FVector3& Translation );
	void Rotate( const FAngles& Euler );
	void Scale( const FVector3& Scaling );

	void Translate( const float& X, const float& Y, const float& Z );
	void Rotate( const float& Pitch, const float& Yaw, const float& Roll );
	void Scale( const float& X, const float& Y, const float& Z );

	/*
		Returns the transform local UP vector.
	*/
	FVector3 GetLocalUp() const;

	/*
		Returns the transform local DOWN vector.
	*/
	FVector3 GetLocalDown() const;

	/*
		Returns the transform local LEFT vector.
	*/
	FVector3 GetLocalLeft() const;

	/*
		Returns the transform local RIGHT vector.
	*/
	FVector3 GetLocalRight() const;

	/*
		Returns the transform local FORWARD vector.
	*/
	FVector3 GetLocalForward() const;

	/*
		Returns the transform local BACKWARD vector.
	*/
	FVector3 GetLocalBackward() const;

	/*
		Rotate the transform to look at a point in space.
	*/
	void LookAt( const FVector3& LookAtPos );

	FMatrix GetLocalMatrix() const;

	FMatrix GetTranslationMatrix() const;
	FMatrix GetRotationMatrix() const;
	FMatrix GetScaleMatrix() const;

	static void RotateVector( FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix );

protected:
	FVector3 m_Position;
	FQuat m_Rotation;
	FVector3 m_Scale;

	FTransform* m_Parent;

};

#include "Transform.inl"
