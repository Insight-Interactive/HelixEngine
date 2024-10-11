// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	Inline function implementations for FTransform class.
*/

FORCEINLINE FTransform::FTransform()
	: m_Position( FVector3::Zero )
	, m_Rotation( FQuat::Identity )
	, m_Scale( FVector3::One )
	, m_Parent( nullptr )
{
}

FORCEINLINE FTransform::~FTransform()
{
}

FORCEINLINE FTransform::FTransform( FTransform&& Other ) noexcept
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

FORCEINLINE /*static*/ void FTransform::RotateVector(FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix)
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

FORCEINLINE void FTransform::Rotate(const FAngles& Euler )
{
	m_Rotation *= FQuat::CreateFromYawPitchRoll( Euler.yaw, Euler.pitch, Euler.roll );
}

FORCEINLINE void FTransform::Rotate(const float& Pitch, const float& Yaw, const float& Roll)
{
	Rotate( FAngles( Pitch, Yaw, Roll ) );
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
	m_Rotation = FQuat::CreateFromYawPitchRoll( Yaw, Pitch, Roll );
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

FORCEINLINE void FTransform::SetRotation(const FAngles& Euler)
{
	SetRotation( Euler.pitch, Euler.yaw, Euler.roll);
}

FORCEINLINE void FTransform::SetRotation( const FQuat& Rotation )
{
	m_Rotation = Rotation;
}

FORCEINLINE void FTransform::SetScale(const FVector3& Scale)
{
	SetScale(Scale.x, Scale.y, Scale.z);
}

FORCEINLINE FVector3 FTransform::GetLocalUp() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Up, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalDown() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Down, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalLeft() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Left, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalRight() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Right, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalForward() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Forward, GetRotationMatrix() );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalBackward() const
{
	FVector3 Result;
	RotateVector( Result, FVector3::Backward, GetRotationMatrix() );
	return Result;
}

FORCEINLINE void FTransform::LookAt(const FVector3& Target)
{
	if (Target == m_Position)
		return;

	FVector3 Direction = m_Position - Target;


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
		Yaw += Math::Pi;

	FQuat PitchDelta = FQuat::CreateFromAxisAngle( FVector3::Right, Pitch );
	FQuat YawDelta = FQuat::CreateFromAxisAngle( FVector3::Up, Yaw );
	SetRotation( PitchDelta * YawDelta );

	//SetRotation(Pitch, Yaw, 0.0f);
}

FORCEINLINE FMatrix FTransform::GetLocalMatrix() const
{
	FMatrix Translation = GetTranslationMatrix();
	FMatrix Rotation = GetRotationMatrix();
	FMatrix Scale = GetScaleMatrix();

	return (Scale * Rotation) * Translation;
}

FORCEINLINE FMatrix FTransform::GetTranslationMatrix() const 
{ 
	return DirectX::XMMatrixTranslationFromVector(m_Position); 
}

FORCEINLINE FMatrix FTransform::GetRotationMatrix() const 
{ 
	return FMatrix::CreateFromQuaternion( m_Rotation );
}

FORCEINLINE FMatrix FTransform::GetScaleMatrix() const 
{ 
	return DirectX::XMMatrixScalingFromVector( m_Scale );
}

