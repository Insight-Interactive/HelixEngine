#pragma once

#include "MathFwd.h"
#include "CoreFwd.h"

#include "MathCore.h"
#include "Containers/TDynamicArray.h"

class MATH_API Transform
{
public:
	Transform::Transform()
		: m_pParent(NULL)
		, m_Position(FVector3::Zero)
		, m_Rotation(FVector3::Zero)
		, m_Scale(FVector3::One)
	{
	}
	Transform::~Transform()
	{
		ReAssignChildrenToParent();
		UnsetParent();
	}
	Transform(Transform&& Other) noexcept
	{
		m_Scale = Other.m_Scale;
		m_Position = Other.m_Position;
		m_Rotation = Other.m_Rotation;
		m_pParent = Other.m_pParent;
		m_Children = Other.m_Children;

		ComputeAllMatriciesAndUpdateChildren();

		Other.m_pParent = NULL;
	}
	Transform(const Transform& Transform)
	{
		*this = Transform;
	}
	Transform& operator = (const Transform& Other)
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
	inline Transform* GetParent() const { return m_pParent; }
	inline void SetParent(Transform* NewParent)
	{
		m_pParent = NewParent;
		if (m_pParent != NULL)
		{
			m_pParent->AddChild(*this);
		}
		ComputeWorldMatrix();
	}
	inline void UnsetParent()
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
	inline FVector3 GetPosition()	const { return m_Position; }
	inline FVector3 GetRotation()	const { return m_Rotation; }
	inline FVector3 GetScale()		const { return m_Scale; }

	/*
		Returns the absolute world position of this actor. As in,
		it's position in world space not relative to it's parent (if it has one).
	*/
	inline FVector3 GetAbsoluteWorldPosition()
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
	inline void SetPosition(float X, float Y, float Z);

	/*
		Set the rotation of the transform using Pitch, Yaw, Roll in degrees relative to its parent.
	*/
	inline void SetRotation(float Pitch, float Yaw, float Roll);

	/*
		Set the scale of the object relative to its parent.
	*/
	inline void SetScale(float X, float Y, float Z);

	inline void SetPosition(const FVector3& Position);
	inline void SetRotation(const FVector3& Rotation);
	inline void SetScale(const FVector3& Scale);

	/*
		Returns the transform local UP vector.
	*/
	inline FVector3 GetLocalUp();

	/*
		Returns the transform local DOWN vector.
	*/
	inline FVector3 GetLocalDown();

	/*
		Returns the transform local LEFT vector.
	*/
	inline FVector3 GetLocalLeft();

	/*
		Returns the transform local RIGHT vector.
	*/
	inline FVector3 GetLocalRight();

	/*
		Returns the transform local FORWARD vector.
	*/
	inline FVector3 GetLocalForward();

	/*
		Returns the transform local BACKWARD vector.
	*/
	inline FVector3 GetLocalBackward();


	inline void Translate(const FVector3& Translation);
	inline void Rotate(const FVector3& Rotation);
	inline void Scale(const FVector3& NewScale);

	inline void Translate(float X, float Y, float Z);
	inline void Rotate(float Pitch, float Yaw, float Roll);
	inline void Scale(float X, float Y, float Z);

	/*
		Transform the point to look at a point in space.
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

	inline void RotateVector(FVector3& outResult, const FVector3& Direction, const FMatrix& Matrix);

	/*
		Add a child that will be updated relative to this transform.
	*/
	inline void AddChild(Transform& Child);
	/*
		Remove a child from being updated form this transform.
	*/
	inline bool RemoveChild(Transform* Child);
	/*
		Repoint the children attached to this tansform to this parent's parent.
	*/
	inline void ReAssignChildrenToParent();
	/*
		Compute the world space matrix of this transform.
	*/
	inline void ComputeWorldMatrix();
	/*
		Update the children relative to this transform.
	*/
	inline void UpdateChildren();


	// Matrix Operations
	//
	void UpdateLocalMatrix();
	void TranslateLocalMatrix();
	void ScaleLocalMatrix();
	void RotateLocalMatrix();
	inline void ComputeAllMatriciesAndUpdateChildren();
	inline void UpdateLocalVectors();

	Transform* m_pParent;
	TDynamicArray<Transform*> m_Children;

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
