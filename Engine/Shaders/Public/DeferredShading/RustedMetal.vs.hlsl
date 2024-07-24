// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "../Core/Common.hlsli"
#include "DeferredShadingCommon.hlsli"


HE_DECLARE_CONSTANT_BUFFER( RustedMetalConsts, 3 )
{
    float2 kUVOffset;
}

//
// Entry Point
//
GP_PSInput main( GP_VSInput Input )
{
    GP_PSInput Result;

    float4x4 WorldView = mul( kWorldMat, kCameraView );
    float4x4 WorldViewProjection = mul( WorldView, kCameraProjection );

    Result.Position = mul( float4(Input.Position, 1.0f), WorldViewProjection );

    Result.UVs = Input.UVs * kUVOffset;
    Result.Normal = Input.Normal;

    CalculateTangentSpaceNormals( Result.TBN, Input.Normal, Input.Tangent, Input.BiTangent, kWorldMat );

    return Result;
}
