// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "DeferredShadingCommon.hlsli"
#include "../../../Source/Runtime/Renderer/Public/CommonMacros.h"



//
// Entry Point
//
GP_PSInputSkinned main( GP_VSInputSkinned Input )
{
    GP_PSInputSkinned Result;

    const float4x4 WorldView = mul( kWorldMat, kCameraView );
    const float4x4 WorldViewProjection = mul( WorldView, kCameraProjection );
    const float4x4 ViewProjection = mul( WorldView, kCameraProjection );


    for (int i = 0; i < R_MAX_JOINTS_PER_VERTEX; i++)
    {
        matrix JointTransform = Joints[Input.JointIDs[i]];
        Result.Position += Input.Weights[i] * mul( float4( Input.Position, 0.f ), JointTransform );
        Result.Normal += Input.Weights[i] * mul( Input.Normal, (float3x3)JointTransform );
    }
    Result.Position = mul( float4( Result.Position.xyz, 1.f ), WorldViewProjection );
    //Result.Position = mul( float4( Input.Position.xyz, 1.f ), WorldViewProjection );

    Result.JointIDs = Input.JointIDs;
    Result.Weights = Input.Weights;
    Result.VertexColor = Input.Color;
    Result.UVs = Input.UVs;

    Result.Tangent = mul( float4(Input.Tangent, 1), WorldView ).xyz;
    Result.BiTangent = mul( float4(Input.BiTangent, 1), WorldView ).xyz;

    return Result;
}
