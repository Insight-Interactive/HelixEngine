// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HCameraComponent.h"

#include "Engine/ViewportContext.h"
#include "World/CameraManager.h"
#include "World/World.h"


HCameraComponent::HCameraComponent(FComponentInitArgs& InitArgs)
	: HSceneComponent(InitArgs)
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

	const float DisplayWidth = GetWorld()->GetWindowWidth();
	const float DisplayHeight = GetWorld()->GetWindowHeight();

	switch (m_ViewProps.ProjectionType)
	{
	case VT_Perspective:
	{
		
		const float FOVRadians = Math::DegreesToRadians( FOVDegrees );
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
		HE_ASSERT( false );
	break;
	}
}

void HCameraComponent::Render(FCommandContext& GfxContext) 
{

}
