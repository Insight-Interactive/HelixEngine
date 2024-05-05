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

    float4x4 WorldView = mul( kWorldMat, kCameraView );
    float4x4 WorldViewProjection = mul( WorldView, kCameraProjection );

    //float4x4 jointTransform = 0;
    //for (int i = 0; i < HE_MAX_JOINTS_PER_VERTEX; i++)
    //{
    //    jointTransform += Joints[ Input.JointIDs[i] ] * Input.Weights[i];
    //}
    //float3 skinnedPos = mul( float4(Input.Position, 1.0f), jointTransform ).xyz;
    //Result.Position = mul( float4(skinnedPos, 1), WorldViewProjection );
    
    float3 totalLocalPos = float3(0, 0, 0);
    for (int i = 0; i < HE_MAX_JOINTS_PER_VERTEX; i++)
    {
        totalLocalPos += Input.Weights[i] * mul( float4(Input.Position, 0), Joints[Input.JointIDs[i]] ).xyz;
    }
    Result.Position = mul( float4(totalLocalPos, 1), WorldViewProjection );

    /*float4 totalLocalPos = float4(0, 0, 0, 0);
    for (int i = 0; i < HE_MAX_JOINTS_PER_VERTEX; i++)
    {
        float4x4 jointTransform = Joints[Input.JointIDs[i]];
        float4 posPosition = mul( float4(Input.Position, 1), jointTransform );
        totalLocalPos += posPosition * Input.Weights[i];

    }
    Result.Position = mul( totalLocalPos, WorldViewProjection );*/


    //Result.Position = mul( float4(Input.Position, 1.0f), WorldViewProjection );

    Result.JointIDs = Input.JointIDs;
    Result.Weights = Input.Weights;
    Result.VertexColor = Input.Color;
    Result.UVs = Input.UVs;

    Result.Normal = normalize( mul( float4(Input.Normal, 0), kWorldMat ) ).xyz;
    Result.Tangent = mul( float4(Input.Tangent, 1), WorldView ).xyz;
    Result.BiTangent = mul( float4(Input.BiTangent, 1), WorldView ).xyz;

    return Result;
}
