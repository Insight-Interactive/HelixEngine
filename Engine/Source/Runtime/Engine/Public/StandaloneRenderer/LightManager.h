#pragma once

#include "StandaloneRenderer/ConstantBufferStructures.h"


class LightManager
{
public:
	LightManager() = default;
	~LightManager() = default;

	/*
		Allocates a spot light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	inline void AllocateSpotLightData(SpotLightDataHandle& OutHandle, SpotLightCBData** ppOutLight);

	/*
		Allocates a point light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	inline void AllocatePointLightData(PointLightDataHandle& OutHandle, PointLightCBData** ppOutLight);

	/*
		Allocates a directional light and adds it to the scene. Populates a handle to the newly created point light.
		@param [out] OutHandle - Handle to the newly allocated light.
		@param [out] ppOutLight - Optional pointer to the light that will be created. Only for use with initialization.
	*/
	inline void AllocateDirectionalLightData(DirectionalLightDataHandle& OutHandle, DirectionalLightCBData** ppOutLight);


	inline SpotLightCBData* GetSpotLightData(SpotLightDataHandle Handle);
	inline PointLightCBData* GetPointLightData(PointLightDataHandle Handle);
	inline DirectionalLightCBData* GetDirectionalLightData(DirectionalLightDataHandle Handle);

	inline SpotLightCBData* GetSpotLightBufferPointer();
	inline PointLightCBData* GetPointLighBufferPointer();
	inline DirectionalLightCBData* GetDirectionalLightBufferPointer();

	inline uint32 GetSceneSpotLightCount();
	inline uint32 GetScenePointLightCount();
	inline uint32 GetSceneDirectionalLightCount();

private:
	inline void InitializeSpotLightData(SpotLightCBData& Light);
	inline void InitializePointLightData(PointLightCBData& Light);
	inline void InitializeDirectionalLightData(DirectionalLightCBData& Light);

	template <typename LightType, typename LightHandleType>
	inline LightType* InternalFindLightByHandle(TDynamicArray<LightType>& LightBuffer, const LightHandleType& Handle);

private:
	TDynamicArray<SpotLightCBData>			m_SceneSpotLightDatas;
	TDynamicArray<PointLightCBData>			m_ScenePointLightDatas;
	TDynamicArray<DirectionalLightCBData>	m_SceneDirectionalLightDatas;

	SpotLightDataHandle					s_NextSpotLightDatahandle;
	PointLightDataHandle				s_NextPointLightDataHandle;
	DirectionalLightDataHandle			s_NextDirectionalLightDataHandle;

};

//
// Inline Function Implementations
//


inline void LightManager::AllocateSpotLightData(SpotLightDataHandle& OutHandle, SpotLightCBData** pOutLight)
{
	if (m_SceneSpotLightDatas.Size() == HE_MAX_SPOT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many spot lights added to the scene!"));

		*pOutLight = NULL;
		OutHandle = IE_INVALID_SPOT_LIGHT_HANDLE;
	}
	SpotLightCBData& NewLight = m_SceneSpotLightDatas.EmplaceBack(SpotLightCBData{});
	InitializeSpotLightData(NewLight);
	NewLight.Id = s_NextSpotLightDatahandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

inline void LightManager::AllocatePointLightData(PointLightDataHandle& OutHandle, PointLightCBData** pOutLight)
{
	if (m_ScenePointLightDatas.Size() == HE_MAX_POINT_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many point lights added to the scene!"));

		*pOutLight = NULL;
		OutHandle = IE_INVALID_POINT_LIGHT_HANDLE;
	}
	PointLightCBData& NewLight = m_ScenePointLightDatas.EmplaceBack(PointLightCBData{});
	InitializePointLightData(NewLight);
	NewLight.Id = s_NextPointLightDataHandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

inline void LightManager::AllocateDirectionalLightData(DirectionalLightDataHandle& OutHandle, DirectionalLightCBData** pOutLight)
{
	if (m_SceneDirectionalLightDatas.Size() == HE_MAX_DIRECTIONAL_LIGHTS)
	{
		HE_LOG(Warning, TEXT("Too many directional lights added to the scene!"));
		*pOutLight = NULL;
		OutHandle = IE_INVALID_DIRECTIONAL_LIGHT_HANDLE;
	}
	DirectionalLightCBData& NewLight = m_SceneDirectionalLightDatas.EmplaceBack(DirectionalLightCBData{});
	InitializeDirectionalLightData(NewLight);
	NewLight.Id = s_NextDirectionalLightDataHandle++;

	if (pOutLight != NULL)
		*pOutLight = &NewLight;

	OutHandle = NewLight.Id;
}

inline SpotLightCBData* LightManager::GetSpotLightData(SpotLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_SceneSpotLightDatas, Handle);
}

inline PointLightCBData* LightManager::GetPointLightData(PointLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_ScenePointLightDatas, Handle);
}

inline DirectionalLightCBData* LightManager::GetDirectionalLightData(DirectionalLightDataHandle Handle)
{
	return InternalFindLightByHandle(m_SceneDirectionalLightDatas, Handle);
}

template <typename LightType, typename LightHandleType>
inline LightType* LightManager::InternalFindLightByHandle(TDynamicArray<LightType>& LightBuffer, const LightHandleType& LightHandle)
{
	HE_ASSERT(LightHandle.IsValid()); // Invalid handle provided when searching for corresponding light!

	for (uint64 i = 0; i < LightBuffer.Size(); ++i)
	{
		if (LightBuffer[i].Id == LightHandle)
		{
			return &LightBuffer[i];
		}
	}
	return NULL;
}

inline PointLightCBData* LightManager::GetPointLighBufferPointer()
{
	return m_ScenePointLightDatas.Data();
}

inline uint32 LightManager::GetScenePointLightCount()
{
	return (uint32)m_ScenePointLightDatas.Size();
}

inline SpotLightCBData* LightManager::GetSpotLightBufferPointer()
{
	return m_SceneSpotLightDatas.Data();
}

inline uint32 LightManager::GetSceneSpotLightCount()
{
	return (uint32)m_SceneSpotLightDatas.Size();
}

inline DirectionalLightCBData* LightManager::GetDirectionalLightBufferPointer()
{
	return m_SceneDirectionalLightDatas.Data();
}

inline uint32 LightManager::GetSceneDirectionalLightCount()
{
	return (uint32)m_SceneDirectionalLightDatas.Size();
}

inline void LightManager::InitializeSpotLightData(SpotLightCBData& Light)
{

}

inline void LightManager::InitializePointLightData(PointLightCBData& Light)
{
	ZeroMemory(&Light, sizeof(PointLightCBData));
	Light.Brightness = kDefaultBrightness;
	Light.Color = FVector4::One;
}

inline void LightManager::InitializeDirectionalLightData(DirectionalLightCBData& Light)
{
	ZeroMemory(&Light, sizeof(DirectionalLightCBData));
	Light.Brightness = kDefaultBrightness;
	Light.Color = FVector4::One;
	Light.Direction = FVector4::One;
}
