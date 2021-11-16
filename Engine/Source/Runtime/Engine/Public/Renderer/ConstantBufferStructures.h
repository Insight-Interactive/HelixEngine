#pragma once

#include "MathCore.h"
#include "Engine/EngineDefines.h"
#include "CommonStructs.h"
#include "Handle.h"

// -----------
// Handles
// -----------
//
typedef Handle SpotLightDataHandle;
typedef Handle PointLightDataHandle;
typedef Handle DirectionalLightDataHandle;

#define IE_INVALID_SPOT_LIGHT_HANDLE		( SpotLightDataHandle		(-1) )
#define IE_INVALID_POINT_LIGHT_HANDLE		( PointLightDataHandle		(-1) )
#define IE_INVALID_DIRECTIONAL_LIGHT_HANDLE	( DirectionalLightDataHandle(-1) )

const float kDefaultBrightness = 20.f;

HE_ALIGN( 16 ) struct SceneConstantsCBData
{
	FMatrix kViewMat;
	FMatrix kProjMat;
	FMatrix kInverseViewMat;
	FMatrix kInverseProjMat;
	FVector3 kCameraPos;
	float kWorldTime;
	float kCameraNearZ;
	float kCameraFarZ;
};
HE_ALIGN( 16 ) struct MeshWorldCBData
{
	// World space model matrix.
	FMatrix kWorldMat;
};
HE_ALIGN( 16 ) struct MaterialConstantsCBData
{
	// Color override.
	FVector4 kColor;
};

// Light Structures
// Keep these in sync with "Shaders/Public/Core/LightsFwd.hlsli"
//
HE_ALIGN( 16 ) struct PointLightCBData
{
	// The position of the light in world space.
	FVector3				Position;

	// The radial influence of the light.
	float Radius;

	// The color of the light. Fourth component is unused.
	FVector4				Color;

	// The brightness the light will illuminate.
	float					Brightness;

	// The unique identifier of the light.
	PointLightDataHandle	Id;
};
HE_ALIGN( 16 ) struct DirectionalLightCBData
{
	// The direction of the light.
	FVector4						Direction;

	// The color of the light.
	FVector4						Color;

	// The brightness the light will illuminate.
	float							Brightness;

	// The unique identifier of the light.
	DirectionalLightDataHandle		Id;
};
HE_ALIGN( 16 ) struct SpotLightCBData
{
	float					TEMP;
	SpotLightDataHandle		Id;
};
struct SceneLightsCBData
{
	uint32					NumPointLights;
	uint32					NumDirectionalLights;
	float					Unused0[2];

	PointLightCBData			PointLights[HE_MAX_POINT_LIGHTS];
	DirectionalLightCBData	DirectionalLights[HE_MAX_DIRECTIONAL_LIGHTS];
	//TODO: SpotLightData			SpotLights[HE_MAX_SPOT_LIGHTS];
};
HE_ALIGN( 16 ) struct PostProcessSettings
{

};

// Input layouts.
extern FInputElementDesc g_SceneMeshInputElements[];
extern const uint32 kNumSceneMeshCommonInputElements;

extern FInputElementDesc g_ScreenSpaceInputElements[];
extern const uint32 kNumScreenSpaceInputElements;

// Sampler Descriptions.
extern FSamplerDesc g_LinearWrapSamplerDesc;
