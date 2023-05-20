// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "ForwardShadingCommon.hlsli"

// Opaque material Textures
//
HE_DECLARE_TEXTURE2D( MaterialAlbedo , 0);
HE_DECLARE_TEXTURE2D( MaterialNormal , 1);


HE_DECLARE_CONSTANT_BUFFER(Translucency, 3)
{
    float kOpacity = 1.0f;
}

// TODO: Reflection fails if not all parameters in the file are used!

//
// Entry Point
//
FP_PSOutput main( FP_PSInput Input )
{
	FP_PSOutput Output;

	// Sample material textures.
    float3 AlbedoSample = MaterialAlbedo.Sample(LinearWrapSampler, Input.UVs).rgb;
    float3 NormalSample = MaterialNormal.Sample(LinearWrapSampler, Input.UVs).rgb;
    
    float3 Albedo = pow( abs( AlbedoSample ), NormalSample );
    Output.LitPixel = float4(Albedo, kOpacity);

    //float3 LightLuminance = PBRLightPixel( AlbedoSample, NormalSample, 0.5, 0.0, 0.1, Input.WorldPos, Input.UVs );
    //float3 Ambient = float3(0.03, 0.03, 0.03) * AlbedoSample;
    //float3 Color = Ambient + LightLuminance;
    //Color = Color / (Color + float3(1, 1, 1));
    //Color = pow( abs( Color ), float3(1 / 2.2, 1 / 2.2, 1 / 2.2) );
    //Output.LitPixel = float4(Color, kOpacity);
    
    return Output;
}
