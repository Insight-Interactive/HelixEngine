// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Actor/Components/HCameraComponent.h"

#include "Engine/ViewportContext.h"
#include "World/CameraManager.h"


HCameraComponent::HCameraComponent(const HName& Name)
	: HActorComponent(Name)
{
	UpdateViewMat();
	SetProjectionValues( kDefaultFOV, 1600.f, 900.f, kDefaultNearZ, kDefaultFarZ );
}

HCameraComponent::~HCameraComponent()
{

}

void HCameraComponent::BeginPlay()
{
}

void HCameraComponent::Tick(float DeltaTime)
{
}

void HCameraComponent::OnCreate()
{
	GetWorld()->GetCameraManager()->CacheCamera( this );
}

void HCameraComponent::OnDestroy()
{
	GetWorld()->GetCameraManager()->RemoveCachedCamera( this );
}

void HCameraComponent::SetProjectionValues(float FOVDegrees, float Width, float Height, float NearZ, float FarZ)
{
	m_ViewProps.NearZ = NearZ;
	m_ViewProps.FarZ = FarZ;

	switch (m_ViewProps.ProjectionType)
	{
	case VT_Perspective:
	{
		const float FOVRadians = FOVDegrees * (3.14f / 180.0f);
		const float AspectRatio = Width / Height;
		m_ViewProps.ProjectionMat = XMMatrixPerspectiveFovLH(FOVRadians, AspectRatio, NearZ, FarZ);
	}
	break;
	case VT_Orthographic:
	{
		m_ViewProps.ProjectionMat = XMMatrixOrthographicLH(Width, Height, NearZ, FarZ);
	}
	break;
	default:
		HE_LOG(Warning, TEXT("Unidentified projection type provided when calculation projection values."))
			break;
	}
}

void HCameraComponent::Render(ICommandContext& GfxContext) 
{

}
