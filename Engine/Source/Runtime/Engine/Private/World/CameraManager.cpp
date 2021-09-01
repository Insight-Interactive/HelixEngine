#include "EnginePCH.h"

#include "World/CameraManager.h"

#include "Engine/ViewportContext.h"
#include "GameFramework/Actor/Components/HCameraComponent.h"


CameraManager::CameraManager( HWorld* pWorld )
	: m_pWorld( pWorld )
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::Tick( float DeltaTime )
{
	for (size_t i = 0; i < m_Cameras.size(); ++i)
	{
		m_Cameras[i]->UpdateViewMat();
	}
}

void CameraManager::CacheCamera( HCameraComponent* pCamera )
{
	HE_ASSERT( pCamera != NULL ); // Trying to cache a null camera.
	m_Cameras.push_back( pCamera );
}

void CameraManager::RemoveCachedCamera( HCameraComponent* pCamera )
{
	for (size_t i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras[i] == pCamera)
		{
			m_Cameras.erase( m_Cameras.begin() + i );
		}
	}
}