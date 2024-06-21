// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "DeferredShadingCommon.hlsli"


// Opaque material Textures
//
HE_DECLARE_TEXTURE2D( Albedo, 0);
HE_DECLARE_TEXTURE2D( Normal, 1);

//
// Entry Point
//
GP_PSOutput main(GP_PSInput Input)
{
	GP_PSOutput Output;

	float3 AlbedoSample = Albedo.Sample(LinearWrapSampler, Input.UVs).rgb;
	float3 NormalSample = Normal.Sample(LinearWrapSampler, Input.UVs).rgb;


	Output.Normal = float4(NormalSample, 1);
    float3 FinalColor = /*AlbedoSample +*/ float3(1, 0, 0) /*+ kMaterialColor.rgb*/;
    Output.Albedo = float4(FinalColor, 0.1);
	
	
    Output.Roughness = 0.5f;
    Output.Metallic = 0.5f;
    Output.Specular = 0.4f;
	
	return Output;
}