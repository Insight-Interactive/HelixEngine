#pragma once

#include "MathFwd.h"
#include "CoreFwd.h"

#include "MathCore.h"


class MATH_API FTransform
{
public:
	FTransform::FTransform()
		: m_pParent(NULL)
		, m_Position(FVector3::Zero)
		, m_Rotation(FVector3::Zero)
		, m_Scale(FVector3::One)
	{
	}
	FTransform::~FTransform()
	{
		ReAssignChildrenToParent();
		UnsetParent();
	}
	FTransform(FTransform&& Other) noexcept
	{
		m_Scale = Other.m_Scale;
		m_Position = Other.m_Position;
		m_Rotation = Other.m_Rotation;
		m_pParent = Other.m_pParent;
		m_Children = Other.m_Children;

		ComputeAllMatriciesAndUpdateChildren();

		Other.m_pParent = NULL;
	}
	FTransform(const FTransform& FTransform)
	{
		*this = FTransform;
	}
	FTransform& operator = (const FTransform& Other)
	{
		m_Position = Other.m_Position;
		m_Scale = Other.m_Scale;
		m_Rotation = Other.m_Rotation;

		ComputeAllMatriciesAndUpdateChildren();
		return *this;
	}


	/*
		Returns the parent of this transform. Null if no parent.
	*/
	FTransform* GetParent() const { return m_pParent; }
	void SetParent(FTransform* NewParent)
	{
		m_pParent = NewParent;
		if (m_pParent != NULL)
		{
			m_pParent->AddChild(*this);
		}
		ComputeWorldMatrix();
	}
	void UnsetParent()
	{
		if (m_pParent != NULL)
		{
			m_pParent->RemoveChild(this);
			m_pParent = NULL;
		}
	}

	/*
		Returns the objects position relative to it's parent.
	*/
	FVector3 GetPosition()	const { return m_Position; }
	FVector3 GetRotation()	const { return m_Rotation; }
	FVector3 GetScale()		const { return m_Scale; }

	/*
		Returns the absolute world position of this actor. As in,
		it's position in world space not relative to it's parent (if it has one).
	*/
	FVector3 GetAbsoluteWorldPosition()
	{
		if (m_pParent != NULL)
		{
			return m_pParent->GetPosition() + m_Position;
		}
		return m_Position;
	}

	/*
		Set the position of the object relative to its parent.
	*/
	void SetPosition(float X, float Y, float Z);

	/*
		Set the rotation of the transform using Pitch, Yaw, Roll in degrees relative to its parent.
	*/
	void SetRotation(float Pitch, float Yaw, float Roll);

	/*
		Set the scale of the object relative to its parent.
	*/
	void SetScale(float X, float Y, float Z);

	void SetPosition(const FVector3& Position);
	void SetRotation(const FVector3& Rotation);
	void SetScale(const FVector3& Scale);

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


	void Translate(const FVector3& Translation);
	void Rotate(const FVector3& Rotation);
	void Scale(const FVector3& NewScale);

	void Translate(float X, float Y, float Z);
	void Rotate(float Pitch, float Yaw, float Roll);
	void Scale(float X, float Y, float Z);

	/*
		FTransform the point to look at a point in space.
	*/
	void LookAt(const FVector3& LookAtPos);

	/*
		Returns objects local matrix.
	*/
	FMatrix GetLocalMatrix() { return m_LocalMatrix; }
	/*
		Returns a reference to the objects local matrix
	*/
	FMatrix& GetLocalMatrixRef() { return m_LocalMatrix; }
	/*
		Set the objects local matrix
	*/
	void SetLocalMatrix(const FMatrix& matrix);
	FMatrix GetLocalMatrixTransposed() const { return XMMatrixTranspose(m_LocalMatrix); }

	/*
		Returns the objects world space matrix
	*/
	FMatrix GetWorldMatrix() { return m_WorldMatrix; }
	/*
		Returns a reference to the objects world space matrix
	*/
	FMatrix& GetWorldMatrixRef() { return m_WorldMatrix; }
	/*
		Set the objects world matrix
	*/
	void SetWorldMatrix(const FMatrix& matrix);
	FMatrix GetWorldMatrixTransposed() const { return XMMatrixTranspose(m_WorldMatrix); }

	FMatrix& GetTranslationMatrixRef() { return m_TranslationMat; }
	FMatrix GetTranslationMatrix() { return m_TranslationMat; }
	void SetTranslationMatrix(FMatrix matrix) { m_TranslationMat = matrix; UpdateLocalMatrix(); }

	FMatrix& GetRotationMatrixRef() { return m_RotationMat; }
	FMatrix GetRotationMatrix() { return m_RotationMat; }
	void SetRotationMatrix(FMatrix matrix) { m_RotationMat = matrix; UpdateLocalMatrix(); }

	FMatrix& GetScaleMatrixRef() { return m_ScaleMat; }
	FMatrix GetScaleMatrix() { return m_ScaleMat; }
	void SetScaleMatrix(FMatrix matrix) { m_ScaleMat = matrix; UpdateLocalMatrix(); }


protected:

	void RotateVector(FVector3& outResult, const FVector3& Direction, const FMatrix& Matrix);

	/*
		Add a child that will be updated relative to this transform.
	*/
	void AddChild(FTransform& Child);
	/*
		Remove a child from being updated form this transform.
	*/
	bool RemoveChild(FTransform* Child);
	/*
		Repoint the children attached to this tansform to this parent's parent.
	*/
	void ReAssignChildrenToParent();
	/*
		Compute the world space matrix of this transform.
	*/
	void ComputeWorldMatrix();
	/*
		Update the children relative to this transform.
	*/
	void UpdateChildren();


	// Matrix Operations
	//
	void UpdateLocalMatrix();
	void TranslateLocalMatrix();
	void ScaleLocalMatrix();
	void RotateLocalMatrix();
	void ComputeAllMatriciesAndUpdateChildren();
	void UpdateLocalVectors();

	FTransform* m_pParent;
	std::vector<FTransform*> m_Children;

	FMatrix m_LocalMatrix = DirectX::XMMatrixIdentity();
	FMatrix m_WorldMatrix = DirectX::XMMatrixIdentity();

	FMatrix m_TranslationMat = DirectX::XMMatrixIdentity();
	FMatrix m_RotationMat = DirectX::XMMatrixIdentity();
	FMatrix m_ScaleMat = DirectX::XMMatrixIdentity();

	FVector3 m_Position;
	FVector3 m_Rotation;
	FVector3 m_Scale;

	FVector3 m_LocalForward = FVector3::Forward;
	FVector3 m_LocalBackward = FVector3::Backward;
	FVector3 m_LocalLeft = FVector3::Left;
	FVector3 m_LocalRight = FVector3::Right;
	FVector3 m_LocalUp = FVector3::Up;
	FVector3 m_LocalDown = FVector3::Down;
};

#include "Transform.inl"
