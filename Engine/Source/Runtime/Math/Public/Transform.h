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
	FTransform( const FVector3& Position, const FVector3& Scale, const FQuat& Rotation );
	FTransform& operator = ( const FTransform& Other );

	FVector3 GetPosition()		const { return m_Position; }
	FQuat GetRotation()			const {	return m_Rotation; }
	FVector3 GetEulerRotation() const { return m_Rotation.ToEulerAngles(); }
	FVector3 GetScale()			const { return m_Scale; }

	void SetPosition( const float& X, const float& Y, const float& Z );
	void SetRotation( const float& Pitch, const float& Yaw, const float& Roll );
	void SetScale( const float& X, const float& Y, const float& Z );

	void SetPosition( const float* pData );
	void SetRotationE( const float* pEulerData );
	void SetRotationQ( const float* pQuatData );
	void SetScale( const float* pData );

	void SetPosition( const FVector3& Position );
	void SetRotation( const FVector3& Euler );
	void SetRotation( const FQuat& Rotation );
	void SetScale( const FVector3& Scale );

	void Translate( const FVector3& Translation );
	void Rotate( const FVector3& Euler );
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

	static FTransform Interpolate( const FTransform& A, const FTransform& B, const float Amount );
	static void RotateVector( FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix );

protected:
	FVector3 m_Position;
	FQuat m_Rotation;
	FVector3 m_Scale;

};

#include "Transform.inl"
