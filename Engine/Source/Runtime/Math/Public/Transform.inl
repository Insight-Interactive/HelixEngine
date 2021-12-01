// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
	Inline function implementations for FTransform class.
*/

FORCEINLINE FTransform::FTransform()
	: m_Position(FVector3::Zero)
	, m_Rotation(FVector3::Zero)
	, m_Scale(FVector3::One)
{
}

FORCEINLINE FTransform::~FTransform()
{
}

FORCEINLINE FTransform::FTransform(FTransform&& Other) noexcept
{
	m_Scale = Other.m_Scale;
	m_Position = Other.m_Position;
	m_Rotation = Other.m_Rotation;

}

FORCEINLINE FTransform::FTransform(const FTransform& FTransform)
{
	*this = FTransform;
}

FORCEINLINE FTransform& FTransform::operator = (const FTransform& Other)
{
	m_Position = Other.m_Position;
	m_Scale = Other.m_Scale;
	m_Rotation = Other.m_Rotation;
	return *this;
}

FORCEINLINE void FTransform::RotateVector(FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix)
{
	outResult = XMVector3TransformCoord(inTarget, inRotationMatrix);
}


FORCEINLINE void FTransform::Translate(const FVector3& Translation)
{
	Translate(Translation.x, Translation.y, Translation.z);
}

FORCEINLINE void FTransform::Translate(const float& X, const float& Y, const float& Z)
{
	SetPosition( m_Position.x + X, m_Position.y + Y, m_Position.z + Z);
}

FORCEINLINE void FTransform::Rotate(const FVector3& Rotation)
{
	Rotate(Rotation.x, Rotation.y, Rotation.z);
}

FORCEINLINE void FTransform::Rotate(const float& Pitch, const float& Yaw, const float& Roll)
{
	m_Rotation.x += Pitch;
	m_Rotation.y += Yaw;
	m_Rotation.z += Roll;
}

FORCEINLINE void FTransform::Scale(const FVector3& Scaling)
{
	Scale(Scaling.x, Scaling.y, Scaling.z);
}

FORCEINLINE void FTransform::Scale(const float& X, const float& Y, const float& Z)
{
	m_Scale.x += X;
	m_Scale.y += Y;
	m_Scale.z += Z;
}

FORCEINLINE void FTransform::SetPosition(const float& X, const float& Y, const float& Z)
{
	m_Position.x = X;
	m_Position.y = Y;
	m_Position.z = Z;
}

FORCEINLINE void FTransform::SetRotation(const float& Pitch, const float& Yaw, const float& Roll)
{
	m_Rotation.x = Pitch;
	m_Rotation.y = Yaw;
	m_Rotation.z = Roll;
}

FORCEINLINE void FTransform::SetScale(const float& X, const float& Y, const float& Z)
{
	m_Scale.x = X;
	m_Scale.y = Y;
	m_Scale.z = Z;
}

FORCEINLINE void FTransform::SetPosition(const FVector3& Position)
{
	SetPosition(Position.x, Position.y, Position.z);
}

FORCEINLINE void FTransform::SetRotation(const FVector3& Rotation)
{
	SetRotation(Rotation.x, Rotation.y, Rotation.z);
}

FORCEINLINE void FTransform::SetScale(const FVector3& Scale)
{
	SetScale(Scale.x, Scale.y, Scale.z);
}

FORCEINLINE FVector3 FTransform::GetLocalUp()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Up, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalDown()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Down, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalLeft()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Left, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalRight()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Right, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalForward()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Forward, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalBackward()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Backward, GetRotationMatrix() );
	return Result;
}

FORCEINLINE void FTransform::LookAt(const FVector3& Target)
{
	if (Target == m_Position)
		return;

	FVector3 Direction = Target - m_Position;


	float Pitch = 0.0f;
	if (Direction.y != 0.0f)
	{
		const float Distance = sqrtf( Direction.x * Direction.x + Direction.z * Direction.z );
		Pitch = atanf( Direction.y / Distance);
	}

	float Yaw = 0.0f;
	if (Direction.x != 0.0f)
		Yaw = atanf( Direction.x / Direction.z );
	if (Direction.z > 0)
		Yaw += DirectX::XM_PI;

	SetRotation(Pitch, Yaw, 0.0f);
}

FORCEINLINE FMatrix FTransform::GetLocalMatrix() const
{
	FMatrix Translation;
	FMatrix Rotation;
	FMatrix Scale;

	Translation = GetTranslationMatrix();
	Rotation = GetRotationMatrix();
	Scale = GetScaleMatrix();

	return Scale * Rotation * Translation;
}

FORCEINLINE FMatrix FTransform::GetTranslationMatrix() const 
{ 
	return DirectX::XMMatrixTranslationFromVector(m_Position); 
}

FORCEINLINE FMatrix FTransform::GetRotationMatrix() const 
{ 
	return DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z); 
}

FORCEINLINE FMatrix FTransform::GetScaleMatrix() const 
{ 
	return DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
}

