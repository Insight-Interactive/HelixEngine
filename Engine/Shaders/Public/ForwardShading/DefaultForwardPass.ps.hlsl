// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "../Core/PBRCommon.hlsli"
#include "ForwardShadingCommon.hlsli"

// Opaque material Textures
//
HE_DECLARE_TEXTURE2D( MaterialAlbedo , 0);
//HE_DECLARE_TEXTURE2D( MaterialNormal , 1);


HE_DECLARE_CONSTANT_BUFFER(Translucency, 3)
{
    float kOpacity = 1.0f;
}


//
// Entry Point
//
FP_PSOutput main( FP_PSInput Input )
{
	FP_PSOutput Output;

	// Sample material textures.
    float4 AlbedoSample = MaterialAlbedo.Sample(LinearWrapSampler, Input.UVs).rgba;
    //float3 NormalSample = MaterialNormal.Sample(LinearWrapSampler, Input.UVs).rgb;
    
    float3 Albedo = pow(abs(AlbedoSample.rgb), float3(2.2, 2.2, 2.2));
    
    Output.LitPixel = float4(Albedo, kOpacity);
    return Output;
}
