// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../../../Source/Runtime/Engine/Public/Engine/EngineDefines.h"
#include "../../../Source/Runtime/Renderer/Public/CommonMacros.h"
#include "LightsFwd.hlsli"


//
// Core Types and Macros for Rendering 
//
#define R_WITH_D3D12 1

// Type wrappers
#if R_WITH_D3D12 // TODO make this macro work to generate const buffers for each platform.

    // Declare a constant buffer to be input to the shader.
#   define HE_DECLARE_CONSTANT_BUFFER(Name, Register) cbuffer Name : register(b##Register)
    // Declare a 2D texture to be input to the shader.
#   define HE_DECLARE_TEXTURE2D(Name, Register) Texture2D Name : register(t##Register);
    // Declare a texture cube to be input to the shader.
#   define HE_DECLARE_TEXTURECUBE(Name, Register) TextureCube Name : register(t##Register);
    // Declare a texture sampler to be input to the shader.
#   define HE_DECLARE_SAMPLERSTATE(Name, Register) SamplerState Name : register(s##Register);

#endif


//
// Constant Buffers
//

HE_DECLARE_CONSTANT_BUFFER(SceneConstants_CB, kSceneConstantsReg)
{
    float4x4 kCameraView;
    float4x4 kCameraProjection;
    float4x4 kInverseViewMat;
    float4x4 kInverseProjMat;
    float3 kCameraPos;
    float kWorldTime;
    float kCameraNearZ;
    float kCameraFarZ;
}

HE_DECLARE_CONSTANT_BUFFER(MeshWorld_CB, kMeshWorldReg)
{
    float4x4 kWorldMat;
}

HE_DECLARE_CONSTANT_BUFFER(SceneLights_CB, kLightsReg)
{
    uint kNumPointLights;
    uint kNumDirectionalLights;
	/* float Unused0[2]; */

    PointLight kPointLights[HE_MAX_POINT_LIGHTS];
	DirectionalLight kDirectionalLights[HE_MAX_DIRECTIONAL_LIGHTS];
}

HE_DECLARE_CONSTANT_BUFFER( SkeletonBones_CB, kSkeletonBonesReg )
{
    float4x4 Joints[HE_MAX_JOINTS_PER_MODEL];
}

//
// Samplers
//

HE_DECLARE_SAMPLERSTATE(LinearWrapSampler, 0);
