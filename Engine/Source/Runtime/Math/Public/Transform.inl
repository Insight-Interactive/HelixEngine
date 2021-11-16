

FORCEINLINE void FTransform::AddChild(FTransform& Child)
{
	m_Children.push_back(&Child);
}

FORCEINLINE bool FTransform::RemoveChild(FTransform* Child)
{
	auto Iter = std::find( m_Children.begin(), m_Children.end(), Child );
	if (Iter != m_Children.end())
	{
		m_Children.erase( Iter );
		return true;
	}
	return false;
}

FORCEINLINE void FTransform::ReAssignChildrenToParent()
{
	if (m_Children.size() == 0) 
		return;

	for (size_t i = 0; i < m_Children.size(); ++i)
	{
		m_Children[i]->SetParent(GetParent());
	}
}

FORCEINLINE void FTransform::ComputeWorldMatrix()
{
	if (m_pParent != NULL)
	{
		m_WorldMatrix = m_LocalMatrix * m_pParent->GetWorldMatrix();
	}
	else
	{
		m_WorldMatrix = m_LocalMatrix;
	}
	UpdateChildren();
}

FORCEINLINE void FTransform::UpdateChildren()
{
	for (size_t i = 0; i < m_Children.size(); ++i)
	{
		m_Children[i]->ComputeWorldMatrix();
	}
}

FORCEINLINE void FTransform::RotateVector(FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix)
{
	outResult = XMVector3TransformCoord(inTarget, inRotationMatrix);
}

FORCEINLINE void FTransform::ComputeAllMatriciesAndUpdateChildren()
{
	TranslateLocalMatrix();
	ScaleLocalMatrix();
	RotateLocalMatrix();
	UpdateLocalMatrix();
	ComputeWorldMatrix();
	UpdateChildren();
}

FORCEINLINE void FTransform::Translate(const FVector3& Translation)
{
	Translate(Translation.x, Translation.y, Translation.z);
}

FORCEINLINE void FTransform::Translate(float X, float Y, float Z)
{
	SetPosition( m_Position.x + X, m_Position.y + Y, m_Position.z + Z);
}

FORCEINLINE void FTransform::Rotate(const FVector3& Rotation)
{
	Rotate(Rotation.x, Rotation.y, Rotation.z);
}

FORCEINLINE void FTransform::Rotate(float Pitch, float Yaw, float Roll)
{
	m_Rotation.x += Pitch;
	m_Rotation.y += Yaw;
	m_Rotation.z += Roll;
	RotateLocalMatrix();
	UpdateLocalMatrix();
}

FORCEINLINE void FTransform::Scale(const FVector3& NewScale)
{
	Scale(NewScale.x, NewScale.y, NewScale.z);
}

FORCEINLINE void FTransform::Scale(float X, float Y, float Z)
{
	m_Scale.x += X;
	m_Scale.y += Y;
	m_Scale.z += Z;
	ScaleLocalMatrix();
	UpdateLocalMatrix();
}

FORCEINLINE void FTransform::SetPosition(float X, float Y, float Z)
{
	m_Position.x = X;
	m_Position.y = Y;
	m_Position.z = Z;
	TranslateLocalMatrix();
	UpdateLocalMatrix();
}

FORCEINLINE void FTransform::SetRotation(float Pitch, float Yaw, float Roll)
{
	m_Rotation.x = Pitch;
	m_Rotation.y = Yaw;
	m_Rotation.z = Roll;
	RotateLocalMatrix();
	UpdateLocalMatrix();
}

FORCEINLINE void FTransform::SetScale(float X, float Y, float Z)
{
	m_Scale.x = X;
	m_Scale.y = Y;
	m_Scale.z = Z;
	ScaleLocalMatrix();
	UpdateLocalMatrix();
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
	RotateVector( Result, FVector3::Up, m_RotationMat );
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalDown()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Down, m_RotationMat);
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalLeft()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Left, m_RotationMat);
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalRight()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Right, m_RotationMat);
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalForward()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Forward, m_RotationMat);
	return Result;
}

FORCEINLINE FVector3 FTransform::GetLocalBackward()
{
	FVector3 Result = FVector3::Zero;
	RotateVector( Result, FVector3::Backward, m_RotationMat);
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

FORCEINLINE void FTransform::SetLocalMatrix(const FMatrix& matrix)
{
	m_LocalMatrix = matrix;
}

FORCEINLINE void FTransform::SetWorldMatrix(const FMatrix& matrix)
{
	m_WorldMatrix = matrix;
}

FORCEINLINE void FTransform::UpdateLocalMatrix()
{
	m_LocalMatrix = m_ScaleMat * m_TranslationMat * m_RotationMat;
	ComputeWorldMatrix();
}

FORCEINLINE void FTransform::TranslateLocalMatrix()
{
	m_TranslationMat = DirectX::XMMatrixTranslationFromVector(m_Position);
}

FORCEINLINE void FTransform::ScaleLocalMatrix()
{
	m_ScaleMat = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
}

FORCEINLINE void FTransform::RotateLocalMatrix()
{
	m_RotationMat = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
}
