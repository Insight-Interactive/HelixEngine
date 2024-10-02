// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../../../Source/Runtime/Engine/Public/Engine/EngineDefines.h"

//
// Common Light Stuctures.
//

struct PointLight
{
    float3 Position;
    float Radius;
    float3 Color;
    float Brightness;
    uint Id;
};

struct DirectionalLight
{
    float4 Direction;
    float4 Color;
    float Brightness;
    uint Id;
};


//
// Core Types and Macros for Rendering 
//
#define R_WITH_D3D12 1

// Type wrappers
#if R_WITH_D3D12 // TODO make this macro work to generate const buffers for each platform.

    // Declare a constant buffer to be input to the shader.
    // NOTE: New constant buffers must start at ECommonRootParamsBuffers::kNumCommonRootParams (See ShaderRegisters.h)
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

    float SCPad[58];
}

HE_DECLARE_CONSTANT_BUFFER(MeshWorld_CB, kMeshWorldReg)
{
    float4x4 kWorldMat;

    float MWPad[46];
}

HE_DECLARE_CONSTANT_BUFFER(SceneLights_CB, kLightsReg)
{
    uint kNumPointLights;
    uint kNumDirectionalLights;

    PointLight kPointLights[HE_MAX_POINT_LIGHTS];
	DirectionalLight kWorldSun;
}

HE_DECLARE_CONSTANT_BUFFER( SkeletonJoints_CB, kSkeletonJointsReg )
{
    float4x4 Joints[HE_MAX_JOINTS_PER_MODEL];
}

//
// Samplers
//

HE_DECLARE_SAMPLERSTATE(LinearWrapSampler, 0);
