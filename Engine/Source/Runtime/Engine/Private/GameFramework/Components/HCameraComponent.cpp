// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HCameraComponent.h"

#include "Engine/ViewportContext.h"
#include "World/CameraManager.h"
#include "World/HWorld.h"


HCameraComponent::HCameraComponent(const HName& Name)
	: HActorComponent(Name)
{
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
	BuildViewMatrix();
	SetProjectionValues( kDefaultFOV, kDefaultNearZ, kDefaultFarZ );

	GetWorld()->GetCameraManager()->CacheCamera( this );
}

void HCameraComponent::OnDestroy()
{
	GetWorld()->GetCameraManager()->RemoveCachedCamera( this );
}

void HCameraComponent::SetProjectionValues(float FOVDegrees, float NearZ, float FarZ)
{
	m_ViewProps.NearZ = NearZ;
	m_ViewProps.FarZ = FarZ;

	const float DisplayWidth = (float)GetWorld()->GetOwningViewport()->GetWindow().GetWidth();
	const float DisplayHeight = (float)GetWorld()->GetOwningViewport()->GetWindow().GetHeight();

	switch (m_ViewProps.ProjectionType)
	{
	case VT_Perspective:
	{
		const float FOVRadians = FOVDegrees * (3.14f / 180.0f);
		const float AspectRatio = DisplayWidth / DisplayHeight;
		m_ViewProps.ProjectionMat = XMMatrixPerspectiveFovLH(FOVRadians, AspectRatio, NearZ, FarZ);
	}
	break;
	case VT_Orthographic:
	{
		m_ViewProps.ProjectionMat = XMMatrixOrthographicLH( DisplayWidth, DisplayHeight, NearZ, FarZ);
	}
	break;
	default:
		HE_LOG( Warning, TEXT( "Unidentified projection type provided when calculation projection values." ) );
	break;
	}
}

void HCameraComponent::Render(ICommandContext& GfxContext) 
{

}
