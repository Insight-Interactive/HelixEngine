// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Renderer/ConstantBufferStructures.h"


class LightManager
{
public:
	LightManager()
		: s_NextSpotLightDatahandle( 0 )
		, s_NextPointLightDataHandle( 0 )
		, s_NextDirectionalLightDataHandle( 0 )
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

	/*
		Allocates a directional light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	void AllocateDirectionalLightData( DirectionalLightDataHandle& OutHandle, DirectionalLightCBData** ppOutLight );


	SpotLightCBData* GetSpotLightData( SpotLightDataHandle Handle);
	PointLightData* GetPointLightData( PointLightDataHandle Handle);
	DirectionalLightCBData* GetDirectionalLightData( DirectionalLightDataHandle Handle );

	SpotLightCBData* GetSpotLightBufferPointer();
	PointLightData* GetPointLighBufferPointer();
	DirectionalLightCBData* GetDirectionalLightBufferPointer();

	uint32 GetSceneSpotLightCount();
	uint32 GetScenePointLightCount();
	uint32 GetSceneDirectionalLightCount();

private:
	void InitializeSpotLightData(SpotLightCBData& Light);
	void InitializePointLightData(PointLightData& Light);
	void InitializeDirectionalLightData(DirectionalLightCBData& Light);

	template <typename LightType, typename LightHandleType>
	LightType* InternalFindLightByHandle( std::vector<LightType>& LightBuffer, const LightHandleType& Handle );

private:
	std::vector<SpotLightCBData>			m_SceneSpotLightDatas;
	std::vector<PointLightData>			m_ScenePointLightDatas;
	std::vector<DirectionalLightCBData>		m_SceneDirectionalLightDatas;

	uint32					s_NextSpotLightDatahandle;
	uint32				s_NextPointLightDataHandle;
	uint32			s_NextDirectionalLightDataHandle;

};

//
// Inline Function Implementations
//

FORCEINLINE void LightManager::FlushLightCache()
{
	m_SceneSpotLightDatas.clear();
	m_ScenePointLightDatas.clear();
	m_SceneDirectionalLightDatas.clear();
}

FORCEINLINE void LightManager::AllocateSpotLightData(SpotLightDataHandle& OutHandle, SpotLightCBData** pOutLight)
{
	if (m_SceneSpotLightDatas.size() == HE_MAX_SPOT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many spot lights added to the scene!"));

		*pOutLight = NULL;
		OutHandle = IE_INVALID_SPOT_LIGHT_HANDLE;
	}
	SpotLightCBData& NewLight = m_SceneSpotLightDatas.emplace_back(SpotLightCBData{});
	InitializeSpotLightData(NewLight);
	NewLight.Id = s_NextSpotLightDatahandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

FORCEINLINE void LightManager::AllocatePointLightData( PointLightDataHandle& OutHandle, PointLightData** pOutLight)
{
	if (m_ScenePointLightDatas.size() == HE_MAX_POINT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many point lights added to the scene!"));

		*pOutLight = NULL;
		OutHandle = -1;
	}
	PointLightData& NewLight = m_ScenePointLightDatas.emplace_back(PointLightData{});
	InitializePointLightData(NewLight);
	NewLight.Id = s_NextPointLightDataHandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

FORCEINLINE void LightManager::AllocateDirectionalLightData(DirectionalLightDataHandle& OutHandle, DirectionalLightCBData** pOutLight)
{
	if (m_SceneDirectionalLightDatas.size() == HE_MAX_DIRECTIONAL_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many directional lights added to the scene!"));
		*pOutLight = NULL;
		OutHandle = IE_INVALID_DIRECTIONAL_LIGHT_HANDLE;
	}
	DirectionalLightCBData& NewLight = m_SceneDirectionalLightDatas.emplace_back(DirectionalLightCBData{});
	InitializeDirectionalLightData(NewLight);
	NewLight.Id = s_NextDirectionalLightDataHandle++;

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

FORCEINLINE DirectionalLightCBData* LightManager::GetDirectionalLightData( DirectionalLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_SceneDirectionalLightDatas, Handle);
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

FORCEINLINE PointLightData* LightManager::GetPointLighBufferPointer()
{
	return m_ScenePointLightDatas.data();
}

FORCEINLINE uint32 LightManager::GetScenePointLightCount()
{
	return (uint32)m_ScenePointLightDatas.size();
}

FORCEINLINE SpotLightCBData* LightManager::GetSpotLightBufferPointer()
{
	return m_SceneSpotLightDatas.data();
}

FORCEINLINE uint32 LightManager::GetSceneSpotLightCount()
{
	return (uint32)m_SceneSpotLightDatas.size();
}

FORCEINLINE DirectionalLightCBData* LightManager::GetDirectionalLightBufferPointer()
{
	return m_SceneDirectionalLightDatas.data();
}

FORCEINLINE uint32 LightManager::GetSceneDirectionalLightCount()
{
	return (uint32)m_SceneDirectionalLightDatas.size();
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
