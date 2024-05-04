// Copyright 2024 Insight Interactive. All Rights Reserved.

//
// Main Pass In/Out Structures
//

// Vertex Stage
struct FP_VSInput
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float3 BiTangent	: BITANGENT;
	float4 Color		: COLOR;
	float2 UVs			: UVS;
};
struct FP_PSInput // VS Ouptut
{
	float4 Position		: SV_POSITION;
	float3 WorldPos		: WORLDPOS;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float3 BiTangent	: BITANGENT;
	float4 VertexColor	: COLOR;
	float2 UVs			: UVS;
};

// Pixel Stage
struct FP_PSOutput
{
	float4 LitPixel	: SV_Target;
};
