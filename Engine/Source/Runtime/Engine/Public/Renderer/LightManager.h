// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Renderer/ConstantBufferStructures.h"


class LightManager
{
public:
	LightManager()
		: m_NextSpotLightDatahandle( 0 )
		, m_NextPointLightDataHandle( 0 )
	{
	}
	~LightManager()
	{
	}

	/*
		Clear all lights in the cache.
	*/
	void FlushLightCache();
	
	/*
		Allocates a spot light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	void AllocateSpotLightData( SpotLightDataHandle& OutHandle, SpotLightCBData** ppOutLight );

	/*
		Allocates a point light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	void AllocatePointLightData( PointLightDataHandle& OutHandle, PointLightData** ppOutLight );

	SpotLightCBData* GetSpotLightData( SpotLightDataHandle Handle);
	PointLightData* GetPointLightData( PointLightDataHandle Handle);
	DirectionalLightCBData* GetWordSunDirectionalLight();

	PointLightData* GetPointLighBufferPointer() { return m_ScenePointLightDatas.data(); }

	uint32 GetSceneSpotLightCount();
	uint32 GetScenePointLightCount();

private:
	void InitializeSpotLightData(SpotLightCBData& Light);
	void InitializePointLightData(PointLightData& Light);
	void InitializeDirectionalLightData(DirectionalLightCBData& Light);

	template <typename LightType, typename LightHandleType>
	LightType* InternalFindLightByHandle( std::vector<LightType>& LightBuffer, const LightHandleType& Handle );

private:
	std::vector<SpotLightCBData>		m_SceneSpotLightDatas;
	std::vector<PointLightData>			m_ScenePointLightDatas;
	DirectionalLightCBData				m_SceneWorldSun;

	uint32				m_NextSpotLightDatahandle;
	uint32				m_NextPointLightDataHandle;

};


//
// Inline Function Implementations
//

FORCEINLINE void LightManager::FlushLightCache()
{
	m_SceneSpotLightDatas.clear();
	m_ScenePointLightDatas.clear();
}

FORCEINLINE void LightManager::AllocateSpotLightData(SpotLightDataHandle& OutHandle, SpotLightCBData** pOutLight)
{
	if (m_SceneSpotLightDatas.size() == HE_MAX_SPOT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many spot lights added to the scene!"));

		*pOutLight = NULL;
		OutHandle = IE_INVALID_SPOT_LIGHT_HANDLE;
		return;
	}
	SpotLightCBData& NewLight = m_SceneSpotLightDatas.emplace_back(SpotLightCBData{});
	InitializeSpotLightData(NewLight);
	NewLight.Id = m_NextSpotLightDatahandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

FORCEINLINE void LightManager::AllocatePointLightData( PointLightDataHandle& OutHandle, PointLightData** pOutLight)
{
	if (m_ScenePointLightDatas.size() == HE_MAX_POINT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many point lights added to the scene!"));

		if (pOutLight != NULL)
			*pOutLight = NULL;
		OutHandle = -1;
		return;
	}
	PointLightData& NewLight = m_ScenePointLightDatas.emplace_back(PointLightData{});
	InitializePointLightData(NewLight);
	NewLight.Id = m_NextPointLightDataHandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

FORCEINLINE SpotLightCBData* LightManager::GetSpotLightData( SpotLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_SceneSpotLightDatas, Handle);
}

FORCEINLINE PointLightData* LightManager::GetPointLightData( PointLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_ScenePointLightDatas, Handle);
}

FORCEINLINE	DirectionalLightCBData* LightManager::GetWordSunDirectionalLight()
{
	return &m_SceneWorldSun;
}

template <typename LightType, typename LightHandleType>
FORCEINLINE LightType* LightManager::InternalFindLightByHandle( std::vector<LightType>& LightBuffer, const LightHandleType& LightHandle )
{
	for (uint64 i = 0; i < LightBuffer.size(); ++i)
	{
		if (LightBuffer[i].Id == LightHandle)
		{
			return &LightBuffer[i];
		}
	}
	return NULL;
}

FORCEINLINE uint32 LightManager::GetScenePointLightCount()
{
	return (uint32)m_ScenePointLightDatas.size();
}

FORCEINLINE uint32 LightManager::GetSceneSpotLightCount()
{
	return (uint32)m_SceneSpotLightDatas.size();
}

FORCEINLINE void LightManager::InitializeSpotLightData(SpotLightCBData& Light)
{

}

FORCEINLINE void LightManager::InitializePointLightData(PointLightData& Light)
{
	ZeroMemory(&Light, sizeof(PointLightData));
	Light.Brightness = 20.f;
	Light.Color = FVector3::One;
}

FORCEINLINE void LightManager::InitializeDirectionalLightData(DirectionalLightCBData& Light)
{
	ZeroMemory(&Light, sizeof(DirectionalLightCBData));
	Light.Brightness = 20.f;
	Light.Color = FVector4::One;
	Light.Direction = FVector4::One;
}
