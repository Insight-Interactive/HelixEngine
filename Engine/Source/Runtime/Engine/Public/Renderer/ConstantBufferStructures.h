// Copyright 2024 Insight Interactive. All Rights Reserved.
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

struct SceneConstantsCBData
{
	FMatrix kViewMat;
	FMatrix kProjMat;
	FMatrix kInverseViewMat;
	FMatrix kInverseProjMat;
	FVector3 kCameraPos;
	float kWorldTime;
	float kCameraNearZ;
	float kCameraFarZ;
	
	float Pad[58];
};
static_assert((sizeof( SceneConstantsCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

struct MeshWorldCBData
{
	// World space model matrix.
	FMatrix kWorldMat;
	
	float Pad[48];
};
static_assert((sizeof( MeshWorldCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

struct MaterialConstantsCBData
{
	// Color override.
	FVector4 kColor;
	
	float Pad[60];
};
static_assert((sizeof( MaterialConstantsCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

struct WireframeParamsCBData
{
	FVector3 kColor;
	float kOpacity;
	
	float Pad[60];
};
static_assert((sizeof( WireframeParamsCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

// Light Structures
// Keep these in sync with "Shaders/Public/Core/LightsFwd.hlsli"
//
struct PointLightCBData
{
	// The position of the light in world space.
	FVector3				Position = FVector3::Zero;

	// The radial influence of the light.
	float					Radius = 10.0f;

	// The color of the light. Fourth component is unused.
	FVector3				Color = FVector3::One;

	// The brightness the light will illuminate.
	float					Brightness = 1.f;

	// The unique identifier of the light.
	PointLightDataHandle	Id;

	float Pad[55];
};
static_assert((sizeof( PointLightCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
struct DirectionalLightCBData
{
	// The direction of the light.
	FVector4						Direction;

	// The color of the light.
	FVector4						Color;

	// The brightness the light will illuminate.
	float							Brightness;

	// The unique identifier of the light.
	DirectionalLightDataHandle		Id;

	float Pad[54];
};
static_assert((sizeof( DirectionalLightCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
struct SpotLightCBData
{
	float					TEMP;
	SpotLightDataHandle		Id;
	
	float Pad[62];
};
static_assert((sizeof( SpotLightCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
struct SceneLightsCBData
{
	uint32					NumPointLights;
	uint32					NumDirectionalLights;
	float					Unused0[2];

	PointLightCBData		PointLights[HE_MAX_POINT_LIGHTS];
	DirectionalLightCBData	DirectionalLights[HE_MAX_DIRECTIONAL_LIGHTS];
	//TODO: SpotLightData			SpotLights[HE_MAX_SPOT_LIGHTS];

	float Pad[60];
};
static_assert((sizeof( SceneLightsCBData ) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
HE_ALIGN( 16 ) struct PostProcessSettings
{

};
// Input layouts.
extern FInputElementDesc g_SceneMeshInputElements[];
extern const uint32 kNumStaticMeshCommonInputElements;

extern FInputElementDesc g_ScreenSpaceInputElements[];
extern const uint32 kNumScreenSpaceInputElements;

// Sampler Descriptions.
extern FSamplerDesc g_LinearWrapSamplerDesc;
