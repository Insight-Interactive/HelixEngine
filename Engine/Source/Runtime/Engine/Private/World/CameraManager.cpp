// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/CameraManager.h"

#include "Engine/ViewportContext.h"
#include "GameFramework/Components/HCameraComponent.h"


HCameraManager::HCameraManager( HWorld* pWorld )
	: m_pWorld( pWorld )
{

}

HCameraManager::~HCameraManager()
{

}

void HCameraManager::Tick( float DeltaTime )
{
	for (size_t i = 0; i < m_Cameras.size(); ++i)
	{
		m_Cameras[i]->BuildViewMatrix();																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																												
	}
}

void HCameraManager::CacheCamera( HCameraComponent* pCamera )
{
	HE_ASSERT( pCamera != NULL ); // Trying to cache a null camera.
	m_Cameras.push_back( pCamera );
}

void HCameraManager::RemoveCachedCamera( HCameraComponent* pCamera )
{
	for (size_t i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras[i] == pCamera)
		{
			m_Cameras.erase( m_Cameras.begin() + i );
		}
	}
}