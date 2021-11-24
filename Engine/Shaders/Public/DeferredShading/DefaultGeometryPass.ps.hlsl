// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "DeferredShadingCommon.hlsli"


// Opaque material textures
//
HE_DECLARE_TEXTURE2D( MaterialAlbedo,	 0);
HE_DECLARE_TEXTURE2D( MaterialNormal,	 1);
HE_DECLARE_TEXTURE2D( MaterialRoughness, 2);
HE_DECLARE_TEXTURE2D( MaterialMetallic,	 3);


//
// Entry Point
//
GP_PSOutput main(GP_PSInput Input)
{
	GP_PSOutput Output;

	float3 AlbedoSample		= MaterialAlbedo.Sample(LinearWrapSampler, Input.UVs).rgb;
	float3 NormalSample		= MaterialNormal.Sample(LinearWrapSampler, Input.UVs).rgb;
    float RoughnessSample	= MaterialRoughness.Sample(LinearWrapSampler, Input.UVs).r;
    float MetallicSample	= MaterialMetallic.Sample(LinearWrapSampler, Input.UVs).r;
	
	const float3x3 TangentToView = float3x3(normalize(Input.Tangent),
											normalize(Input.BiTangent),
													  Input.Normal);
	float3 NormalTangentSpace;
	NormalTangentSpace.x =  NormalSample.x * 2.0f - 1.0f;
	NormalTangentSpace.y = -NormalSample.y * 2.0f + 1.0f;
	NormalTangentSpace.z =  NormalSample.z;
	NormalTangentSpace	 = normalize(mul(NormalTangentSpace, TangentToView));

	Output.Normal = float4(NormalTangentSpace, 1);
    Output.Albedo = float4(AlbedoSample, 1) ;
	
    Output.Roughness = RoughnessSample;
    Output.Metallic	 = MetallicSample;
    Output.Specular	 = 0.04f;
	
	return Output;
}