// Copyright 2021 Insight Interactive. All Rights Reserved.
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

	FVector3 GetPosition()	const { return m_Position; }
	FVector3 GetRotation()	const { return m_Rotation; }
	FVector3 GetScale()		const { return m_Scale; }

	void SetPosition(const float& X, const float& Y, const float& Z);
	void SetRotation(const float& Pitch, const float& Yaw, const float& Roll);
	void SetScale(const float& X, const float& Y, const float& Z);

	void SetPosition(const FVector3& Position);
	void SetRotation(const FVector3& Rotation);
	void SetScale(const FVector3& Scale);

	void Translate(const FVector3& Translation);
	void Rotate(const FVector3& Rotation);
	void Scale(const FVector3& Scaling);

	void Translate(const float& X, const float& Y, const float& Z);
	void Rotate(const float& Pitch, const float& Yaw, const float& Roll);
	void Scale(const float& X, const float& Y, const float& Z);

	/*
		Returns the transform local UP vector.
	*/
	FVector3 GetLocalUp();

	/*
		Returns the transform local DOWN vector.
	*/
	FVector3 GetLocalDown();

	/*
		Returns the transform local LEFT vector.
	*/
	FVector3 GetLocalLeft();

	/*
		Returns the transform local RIGHT vector.
	*/
	FVector3 GetLocalRight();

	/*
		Returns the transform local FORWARD vector.
	*/
	FVector3 GetLocalForward();

	/*
		Returns the transform local BACKWARD vector.
	*/
	FVector3 GetLocalBackward();

	/*
		Rotate the transform to look at a point in space.
	*/
	void LookAt(const FVector3& LookAtPos);

	FMatrix GetLocalMatrix() const;


	FMatrix GetTranslationMatrix() const;
	FMatrix GetRotationMatrix() const;
	FMatrix GetScaleMatrix() const;

protected:
	void RotateVector(FVector3& outResult, const FVector3& Direction, const FMatrix& Matrix);

protected:
	FVector3 m_Position;
	FVector3 m_Rotation;
	FVector3 m_Scale;

};

#include "Transform.inl"
