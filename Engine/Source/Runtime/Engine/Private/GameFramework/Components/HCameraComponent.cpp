// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HCameraComponent.h"

#include "Engine/ViewportContext.h"
#include "World/CameraManager.h"
#include "World/World.h"
#include "Engine/Engine.h"
#include "ThreadPool.h"
#include "SystemTime.h"


HCameraComponent::HCameraComponent(FComponentInitArgs& InitArgs)
	: HActorComponent(InitArgs)
	, m_FieldOfView( kDefaultFOV )
	, m_ShouldLerpFOV( false )
{
}

HCameraComponent::~HCameraComponent()
{

}

void HCameraComponent::BeginPlay()
{
}

void HCameraComponent::LerpFieldOfView( float NewFOV, float TimeSeconds )
{
	m_FOVLerpParams.StartFOV = m_FieldOfView;
	m_FOVLerpParams.EndFOV = NewFOV;
	m_FOVLerpParams.TimeSeconds = TimeSeconds;
	m_FOVLerpParams.TimeCounter = 0.f;
	m_ShouldLerpFOV = true;
}

void HCameraComponent::Tick(float DeltaTime)
{
	if (m_ShouldLerpFOV)
	{
		m_FOVLerpParams.TimeCounter += DeltaTime;
		float NewFOV = Math::LerpForTime( m_FOVLerpParams.StartFOV, m_FOVLerpParams.EndFOV, m_FOVLerpParams.TimeCounter, m_FOVLerpParams.TimeSeconds );

		if (m_FOVLerpParams.TimeCounter > m_FOVLerpParams.TimeSeconds)
			m_ShouldLerpFOV = false;

		SetFieldOfView( NewFOV );
	}
}

void HCameraComponent::OnCreate()
{
	BuildViewMatrix();
	SetProjectionValues( m_FieldOfView, m_ViewProps.NearZ, m_ViewProps .FarZ );

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
