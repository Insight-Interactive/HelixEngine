

inline void Transform::AddChild(Transform& Child)
{
	m_Children.PushBack(&Child);
}

inline bool Transform::RemoveChild(Transform* Child)
{
	for (uint64 i = 0; i < m_Children.Size(); ++i)
	{
		if (m_Children[i] == Child)
		{
			m_Children.RemoveAt(i);
			return true;
		}
	}
	return false;
}

inline void Transform::ReAssignChildrenToParent()
{
	if (m_Children.Size() == 0) 
		return;

	for (size_t i = 0; i < m_Children.Size(); ++i)
	{
		m_Children[i]->SetParent(GetParent());
	}
}

inline void Transform::ComputeWorldMatrix()
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

inline void Transform::UpdateChildren()
{
	for (size_t i = 0; i < m_Children.Size(); ++i)
	{
		m_Children[i]->ComputeWorldMatrix();
	}
}

inline void Transform::RotateVector(FVector3& outResult, const FVector3& inTarget, const FMatrix& inRotationMatrix)
{
	outResult = XMVector3TransformCoord(inTarget, inRotationMatrix);
}

inline void Transform::ComputeAllMatriciesAndUpdateChildren()
{
	TranslateLocalMatrix();
	ScaleLocalMatrix();
	RotateLocalMatrix();
	UpdateLocalMatrix();
	ComputeWorldMatrix();
	UpdateChildren();
}

inline void Transform::UpdateLocalVectors()
{
	RotateVector(m_LocalUp, FVector3::Up, m_RotationMat);
	RotateVector(m_LocalDown, FVector3::Down, m_RotationMat);
	RotateVector(m_LocalLeft, FVector3::Left, m_RotationMat);
	RotateVector(m_LocalRight, FVector3::Right, m_RotationMat);
	RotateVector(m_LocalForward, FVector3::Forward, m_RotationMat);
	RotateVector(m_LocalBackward, FVector3::Backward, m_RotationMat);
}

inline void Transform::Translate(const FVector3& Translation)
{
	Translate(Translation.x, Translation.y, Translation.z);
}

inline void Transform::Translate(float X, float Y, float Z)
{
	float NewX = m_Position.x + X;
	float NewY = m_Position.x + Y;
	float NewZ = m_Position.x + Z;
	SetPosition(NewX, NewY, NewZ);
}

inline void Transform::Rotate(const FVector3& Rotation)
{
	Rotate(Rotation.x, Rotation.y, Rotation.z);
}

inline void Transform::Rotate(float Pitch, float Yaw, float Roll)
{
	m_Rotation.x += Pitch;
	m_Rotation.y += Yaw;
	m_Rotation.z += Roll;
	RotateLocalMatrix();
	UpdateLocalMatrix();
}

inline void Transform::Scale(const FVector3& NewScale)
{
	Scale(NewScale.x, NewScale.y, NewScale.z);
}

inline void Transform::Scale(float X, float Y, float Z)
{
	m_Scale.x += X;
	m_Scale.y += Y;
	m_Scale.z += Z;
	ScaleLocalMatrix();
	UpdateLocalMatrix();
}

inline void Transform::SetPosition(float X, float Y, float Z)
{
	m_Position.x = X;
	m_Position.y = Y;
	m_Position.z = Z;
	TranslateLocalMatrix();
	UpdateLocalMatrix();
}

inline void Transform::SetRotation(float Pitch, float Yaw, float Roll)
{
	m_Rotation.x = Pitch;
	m_Rotation.y = Yaw;
	m_Rotation.z = Roll;
	RotateLocalMatrix();
	UpdateLocalMatrix();
}

inline void Transform::SetScale(float X, float Y, float Z)
{
	m_Scale.x = X;
	m_Scale.y = Y;
	m_Scale.z = Z;
	ScaleLocalMatrix();
	UpdateLocalMatrix();
}

inline void Transform::SetPosition(const FVector3& Position)
{
	SetPosition(Position.x, Position.y, Position.z);
}

inline void Transform::SetRotation(const FVector3& Rotation)
{
	SetRotation(Rotation.x, Rotation.y, Rotation.z);
}

inline void Transform::SetScale(const FVector3& Scale)
{
	SetScale(m_Scale.x, m_Scale.y, m_Scale.z);
}

inline FVector3 Transform::GetLocalUp()
{
	RotateVector(m_LocalUp, FVector3::Up, m_RotationMat);
	return m_LocalUp;
}

inline FVector3 Transform::GetLocalDown()
{
	RotateVector(m_LocalDown, FVector3::Down, m_RotationMat);
	return m_LocalDown;
}

inline FVector3 Transform::GetLocalLeft()
{
	RotateVector(m_LocalLeft, FVector3::Left, m_RotationMat);
	return m_LocalLeft;
}

inline FVector3 Transform::GetLocalRight()
{
	RotateVector(m_LocalRight, FVector3::Right, m_RotationMat);
	return m_LocalRight;
}

inline FVector3 Transform::GetLocalForward()
{
	RotateVector(m_LocalForward, FVector3::Forward, m_RotationMat);
	return m_LocalForward;
}

inline FVector3 Transform::GetLocalBackward()
{
	RotateVector(m_LocalBackward, FVector3::Backward, m_RotationMat);
	return m_LocalBackward;
}


inline void Transform::LookAt(const FVector3& target)
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

inline void Transform::SetLocalMatrix(const FMatrix& matrix)
{
	m_LocalMatrix = matrix;
}

inline void Transform::SetWorldMatrix(const FMatrix& matrix)
{
	m_WorldMatrix = matrix;
}

inline void Transform::UpdateLocalMatrix()
{
	m_LocalMatrix = m_ScaleMat * m_TranslationMat * m_RotationMat;
	ComputeWorldMatrix();
}

inline void Transform::TranslateLocalMatrix()
{
	m_TranslationMat = DirectX::XMMatrixTranslationFromVector(m_Position);
}

inline void Transform::ScaleLocalMatrix()
{
	m_ScaleMat = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
}

inline void Transform::RotateLocalMatrix()
{
	m_RotationMat = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
}
