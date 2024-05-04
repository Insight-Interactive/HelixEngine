// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"

struct VS_Input
{
    float4 Pos      : POSITION;
    float4 UVs      : TEXCOORD;
    float4 Color    : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos      : SV_POSITION;
    float4 Color    : COLOR;
    float2 UVs      : TEXCOORD;
};

VS_OUTPUT main( VS_Input Input, uint VertexID : SV_VertexID )
{
    VS_OUTPUT Result;

    // vert id 0 = 0000, UV = (0, 0)
    // vert id 1 = 0001, UV = (1, 0)
    // vert id 2 = 0010, UV = (0, 1)
    // vert id 3 = 0011, UV = (1, 1)
    float2 UV = float2(VertexID & 1, (VertexID >> 1) & 1);

    // Set the position for the vertex based on which vertex it is (UV)
    Result.Pos = float4(Input.Pos.x + (Input.Pos.z * UV.x), Input.Pos.y - (Input.Pos.w * UV.y), 0, 1);
    Result.Color = Input.Color;

    // Set the texture coordinate based on which vertex it is (UV)
    Result.UVs = float2(Input.UVs.x + (Input.UVs.z * UV.x), Input.UVs.y + (Input.UVs.w * UV.y));

	return Result;
}