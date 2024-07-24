// Copyright 2024 Insight Interactive. All Rights Reserved.

//
// Geometry Pass In/Out Structures
//

// Vertex Stage
struct GP_VSInput
{
	float3 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float3 BiTangent	: BITANGENT;
	float2 UVs			: UVS;
};
struct GP_PSInput // VS Output
{
	float4 Position		: SV_POSITION;
	float3 Normal		: NORMAL;
	float2 UVs			: UVS;

	float3x3 TBN : TBN;
};

// Pixel Stage
struct GP_PSOutput
{
	// GBuffer

	float4 Albedo		: SV_Target0;
	float4 Normal		: SV_Target1;
	float1 Roughness	: SV_Target2;
	float1 Metallic		: SV_Target3;
	float1 Specular		: SV_Target4;
	//float1 Emissive		: SV_Target5;
};


// 
// Light Pass In/Out Structures
//

// Vertex Stage
struct LP_VSInput
{
	float2 Position : POSITION;
	float2 UVs		: UVS;
};
struct LP_PSInput
{
	float4 Position : SV_POSITION;
	float2 UVs		: UVS;
};

// Pixel Stage
struct LP_PSOutput
{
	float4 Result : SV_Target;
};
