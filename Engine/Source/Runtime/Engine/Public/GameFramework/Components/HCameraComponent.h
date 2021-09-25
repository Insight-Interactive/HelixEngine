// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Transform.h"
#include "GameFramework/Components/HActorComponent.h"

const float kDefaultFOV = 75.f;
const float kDefaultNearZ = 0.001f;
const float kDefaultFarZ = 1000.f;

enum EViewType
{
	VT_Perspective,
	VT_Orthographic,
};

struct ProjectionProperties
{
	const float kDefaultNearZ = 0.01f;
	const float kDefaultFarZ = 1000.f;

	constexpr ProjectionProperties()
		: NearZ( kDefaultNearZ )
		, FarZ( kDefaultFarZ )
		, ViewMat( FMatrix::Identity )
		, ProjectionMat( FMatrix::Identity )
		, ProjectionType( VT_Perspective )
	{
	}
	float NearZ;
	float FarZ;
	FMatrix ViewMat;
	FMatrix ProjectionMat;
	EViewType ProjectionType;
};

HCOMPONENT()
class HCameraComponent : public HActorComponent
{
	friend class CameraManager;
public:
	HCameraComponent( const HName& Name );
	virtual ~HCameraComponent();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	void SetProjectionValues( float FOVDegrees, float NearZ, float FarZ );

	FTransform& GetTransform();
	const FMatrix& GetViewMatrix();
	const FMatrix& GetProjectionMatrix();
	float GetNearZ();
	float GetFarZ();
	float GetFieldOfView();
	void SetNearZ( float Value );
	void SetFarZ( float Value );
	void SetFieldOfView( float Value );

protected:
	virtual void Render( ICommandContext& GfxContext ) override;

private:
	void BuildViewMatrix();

protected:
	FTransform m_Transform;
	ProjectionProperties m_ViewProps;
	float m_FieldOfView;

};


//
// Inline function implementations
//

inline FTransform& HCameraComponent::GetTransform()
{
	return m_Transform;
}

inline const FMatrix& HCameraComponent::GetViewMatrix()
{
	return m_ViewProps.ViewMat;
}

inline const FMatrix& HCameraComponent::GetProjectionMatrix()
{
	return m_ViewProps.ProjectionMat;
}

inline float HCameraComponent::GetNearZ()
{
	return m_ViewProps.NearZ;
}

inline float HCameraComponent::GetFarZ()
{
	return m_ViewProps.FarZ;
}

inline float HCameraComponent::GetFieldOfView()
{
	return m_FieldOfView;
}

inline void HCameraComponent::SetNearZ( float Value )
{
	m_ViewProps.NearZ = Value;
}

inline void HCameraComponent::SetFarZ( float Value )
{
	m_ViewProps.FarZ = Value;
}

inline void HCameraComponent::SetFieldOfView( float Value )
{
	m_FieldOfView = Value;
}

inline void HCameraComponent::BuildViewMatrix()
{
	m_ViewProps.ViewMat = XMMatrixLookAtLH(
		m_Transform.GetAbsoluteWorldPosition()
		, m_Transform.GetAbsoluteWorldPosition() + m_Transform.GetLocalForward()
		, m_Transform.GetLocalUp()
	);
}
