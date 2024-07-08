// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
    Puts a value back into normalized device coordinates.
*/
#define ToNDC(Value) Value * 2 - 1

#define PI 3.14159265359

float3 ConstructWorldPosFromDepth(float Depth, float2 TexCoords, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
    float Z = (Depth);
        
    float2 ClipSpaceCoords;
    ClipSpaceCoords.x = (TexCoords.x) * 2 - 1;
    ClipSpaceCoords.y = (1 - TexCoords.y) * 2 - 1;
    
    float4 ClipSpacePos = float4(ClipSpaceCoords, Z, 1.f);
    float4 ViewSpacePos = mul(ClipSpacePos, InverseProjectionMatrix);

    ViewSpacePos /= ViewSpacePos.w;

    float4 WorldSpacePos = mul(ViewSpacePos, InverseViewMatrix);

    return WorldSpacePos.xyz;
}

float LinearizeDepth(float Depth, float CameraNearZ, float CameraFarZ)
{
    float z = ToNDC(Depth);
    return (2.f * CameraNearZ * CameraFarZ) / (CameraFarZ + CameraNearZ - z * (CameraFarZ - CameraNearZ)) / CameraFarZ;
}

void CalculateTangentSpaceNormals( out float3x3 TBN, float3 Normal, float3 Tangent, float3 BiTangent, float4x4 WorldMatrix )
{
    float3 T = normalize( float3(mul( float4(Tangent, 0.f), WorldMatrix ).xyz) );
    float3 B = normalize( float3(mul( float4(BiTangent, 0.f), WorldMatrix ).xyz) );
    float3 N = normalize( float3(mul( float4(Normal, 0.f), WorldMatrix ).xyz) );
    TBN = float3x3(T, B, N);
}
