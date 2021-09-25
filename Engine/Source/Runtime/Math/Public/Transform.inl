

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

FORCEINLINE void FTransform::UpdateLocalVectors()
{
	RotateVector(m_LocalUp, FVector3::Up, m_RotationMat);
	RotateVector(m_LocalDown, FVector3::Down, m_RotationMat);
	RotateVector(m_LocalLeft, FVector3::Left, m_RotationMat);
	RotateVector(m_LocalRight, FVector3::Right, m_RotationMat);
	RotateVector(m_LocalForward, FVector3::Forward, m_RotationMat);
	RotateVector(m_LocalBackward, FVector3::Backward, m_RotationMat);
}

FORCEINLINE void FTransform::Translate(const FVector3& Translation)
{
	Translate(Translation.x, Translation.y, Translation.z);
}

FORCEINLINE void FTransform::Translate(float X, float Y, float Z)
{
	float NewX = m_Position.x + X;
	float NewY = m_Position.x + Y;
	float NewZ = m_Position.x + Z;
	SetPosition(NewX, NewY, NewZ);
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
	RotateVector(m_LocalUp, FVector3::Up, m_RotationMat);
	return m_LocalUp;
}

FORCEINLINE FVector3 FTransform::GetLocalDown()
{
	RotateVector(m_LocalDown, FVector3::Down, m_RotationMat);
	return m_LocalDown;
}

FORCEINLINE FVector3 FTransform::GetLocalLeft()
{
	RotateVector(m_LocalLeft, FVector3::Left, m_RotationMat);
	return m_LocalLeft;
}

FORCEINLINE FVector3 FTransform::GetLocalRight()
{
	RotateVector(m_LocalRight, FVector3::Right, m_RotationMat);
	return m_LocalRight;
}

FORCEINLINE FVector3 FTransform::GetLocalForward()
{
	RotateVector(m_LocalForward, FVector3::Forward, m_RotationMat);
	return m_LocalForward;
}

FORCEINLINE FVector3 FTransform::GetLocalBackward()
{
	RotateVector(m_LocalBackward, FVector3::Backward, m_RotationMat);
	return m_LocalBackward;
}


FORCEINLINE void FTransform::LookAt(const FVector3& target)
{
	// Verify that look at pos is not the same as Model pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior
	if (target == m_Position)
	{
		return;
	}

	float pitch = 0.0f;
	if (target.y != 0.0f)
	{
		const float distance = sqrtf(target.x * target.x + target.z * target.z);
		pitch = atanf(target.y / distance);
	}

	float yaw = 0.0f;
	if (target.x != 0.0f)
	{
		yaw = atanf(target.x / target.z);
	}
	if (target.z > 0)
	{
		yaw += DirectX::XM_PI;
	}

	SetRotation(pitch, yaw, 0.0f);
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
