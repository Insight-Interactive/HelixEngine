// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "DeferredShadingCommon.hlsli"


//
// Entry Point
//
GP_PSInput main(GP_VSInput Input)
{
    GP_PSInput Result;

    float4x4 WorldView = mul(kWorldMat, kCameraView);
    float4x4 WorldViewProjection = mul(WorldView, kCameraProjection);
    
    Result.Position = mul(float4(Input.Position, 1.f), WorldViewProjection);
    Result.Normal = Input.Normal;

    Result.VertexColor = Input.Color;
    Result.UVs = Input.UVs;

    float3 T = normalize( float3(mul( float4(Input.Tangent, 1.f), kWorldMat ).xyz) );
    float3 B = normalize( float3(mul( float4(Input.BiTangent, 1.f), kWorldMat ).xyz) );
    float3 N = normalize( float3(mul( float4(Input.Normal, 1.f), kWorldMat ).xyz) );
    Result.TBN = float3x3(T, B, N);

    float2 Tiling = float2(1.0f, 1.f);
    float2 UVOffset = float2(kWorldTime * 0.3f, 0.f);
    //Result.UVs = float2((Input.UVs.x + UVOffset.x) * Tiling.x, (Input.UVs.y + UVOffset.y) * Tiling.y);
    
    return Result;
}
