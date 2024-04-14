// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "DeferredShadingCommon.hlsli"


// Opaque material Textures
//
//HE_DECLARE_CONSTANT_BUFFER( MeshWorld_CB, 4 )
//{
//	float4 kColor;
//}

//
// Entry Point
//
GP_PSOutput main( GP_PSInput Input )
{
	GP_PSOutput Output;

	const float3x3 TangentToView = float3x3(normalize( Input.Tangent ),
		normalize( Input.BiTangent ),
		Input.Normal);
	float3 NormalTangentSpace;
	NormalTangentSpace.x = Input.Normal.x * 2.0f - 1.0f;
	NormalTangentSpace.y = -Input.Normal.y * 2.0f + 1.0f;
	NormalTangentSpace.z = Input.Normal.z;
	NormalTangentSpace = normalize( mul( NormalTangentSpace, TangentToView ) );

	Output.Normal = float4(NormalTangentSpace, 1);
	Output.Albedo = float4(0.9, 0.9, 0.9, 1);

	Output.Roughness = 0.5f;
	Output.Metallic = 0.5f;
	Output.Specular = 0.4f;

	return Output;
}