// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Transform.h"
#include "GameFramework/Components/HSceneComponent.h"

const float kDefaultFOV = 75.f;
const float kDefaultNearZ = 0.1f;
const float kDefaultFarZ = 1000.f;

enum EViewType
{
	VT_Perspective,
	VT_Orthographic,
};

struct ProjectionProperties
{
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


struct FOVLerpParams
{
	float StartFOV;
	float EndFOV;
	float TimeSeconds;
	
	float TimeCounter;
};

HCOMPONENT()
class HCameraComponent : public HSceneComponent
{
	friend class HCameraManager;
public:
	HE_COMPONENT_GENERATED_BODY( HCameraComponent )

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	void SetProjectionValues( float FOVDegrees, float NearZ, float FarZ );

	const FMatrix& GetViewMatrix();
	const FMatrix& GetProjectionMatrix();
	float GetNearZ() const;
	float GetFarZ() const;
	float GetFieldOfView() const;
	void SetNearZ( float Value );
	void SetFarZ( float Value );
	void SetFieldOfView( float Value );
	void LerpFieldOfView( float NewFOV, float TimeSeconds );

protected:
	virtual void Render( FCommandContext& GfxContext ) override;

private:
	void BuildViewMatrix();


protected:
	ProjectionProperties m_ViewProps;
	float m_FieldOfView;

	FOVLerpParams m_FOVLerpParams;
	bool m_ShouldLerpFOV;
	
};


//
// Inline function implementations
//

inline const FMatrix& HCameraComponent::GetViewMatrix()
{
	return m_ViewProps.ViewMat;
}

inline const FMatrix& HCameraComponent::GetProjectionMatrix()
{
	return m_ViewProps.ProjectionMat;
}

inline float HCameraComponent::GetNearZ() const
{
	return m_ViewProps.NearZ;
}

inline float HCameraComponent::GetFarZ() const
{
	return m_ViewProps.FarZ;
}

inline float HCameraComponent::GetFieldOfView() const
{
	return m_FieldOfView;
}

inline void HCameraComponent::SetNearZ( float Value )
{
	m_ViewProps.NearZ = Value;
	SetProjectionValues( Value, m_ViewProps.NearZ, m_ViewProps.FarZ );
}

inline void HCameraComponent::SetFarZ( float Value )
{
	m_ViewProps.FarZ = Value;
	SetProjectionValues( Value, m_ViewProps.NearZ, m_ViewProps.FarZ );
}

inline void HCameraComponent::SetFieldOfView( float Value )
{
	m_FieldOfView = Value;
	SetProjectionValues( Value, m_ViewProps.NearZ, m_ViewProps.FarZ );
}

inline void HCameraComponent::BuildViewMatrix()
{
	const FVector3 WorldPos = GetWorldMatrix().Translation();
	m_ViewProps.ViewMat = XMMatrixLookAtLH(
		WorldPos, 
		WorldPos + GetLocalForward(),
		GetLocalUp()
	);
}
