#include "../../../Source/Runtime/Engine/Public/Engine/EngineDefines.h"
#include "LightsFwd.hlsli"

//
// Core Types and Macros for Rendering 
//

// Constant Buffers
cbuffer SceneConstants      : register(kSceneConstantsReg)
{
    float4x4 ViewMat;
    float4x4 ProjMat;
    float4x4 InverseViewMat;
    float4x4 InverseProjMat;
    float3 CameraPos;
    float3 ViewVector;
    float WorldTime;
    float CameraNearZ;
    float CameraFarZ;
}
cbuffer MeshWorld           : register(kMeshWorldReg)
{
    float4x4 WorldMat;
}
cbuffer MaterialConstants   : register(kMaterialReg)
{
    float4 MatColor;
};
cbuffer SceneLights         : register(kLightsReg)
{
    uint NumPointLights;
    uint NumDirectionalLights;
	/* float Unused0[2]; */

    PointLight PointLights[HE_MAX_POINT_LIGHTS];
	DirectionalLight DirectionalLights[HE_MAX_DIRECTIONAL_LIGHTS];
}
