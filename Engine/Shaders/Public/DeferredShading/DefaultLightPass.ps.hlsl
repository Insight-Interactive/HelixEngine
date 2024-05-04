// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "DeferredShadingCommon.hlsli"


// GBuffer Inputs
//
HE_DECLARE_TEXTURE2D( SceneDepth        , 0);
HE_DECLARE_TEXTURE2D( AlbedoGBuffer     , 1);
HE_DECLARE_TEXTURE2D( NormalGBuffer     , 2);
HE_DECLARE_TEXTURE2D( RoughnessGBuffer  , 3);
HE_DECLARE_TEXTURE2D( MetallicGBuffer   , 4);
HE_DECLARE_TEXTURE2D( SpecularGBuffer   , 5);


//
// Entry Point
//
LP_PSOutput main(LP_PSInput Input)
{
	LP_PSOutput Output;
	
	// Sample G-Buffer.
    float3 AlbedoSample		= AlbedoGBuffer.Sample(LinearWrapSampler, Input.UVs).rgb;
	float3 NormalSample		= NormalGBuffer.Sample(LinearWrapSampler, Input.UVs).rgb;
	float RoughnessSample	= RoughnessGBuffer.Sample(LinearWrapSampler, Input.UVs).r;
	float MetallicSample	= MetallicGBuffer.Sample(LinearWrapSampler, Input.UVs).r;
	float SpecularSample	= SpecularGBuffer.Sample(LinearWrapSampler, Input.UVs).r;
	float Depth				= SceneDepth.Sample(LinearWrapSampler, Input.UVs).r;
	float3 WorldPos			= ConstructWorldPosFromDepth(Depth, Input.UVs, kInverseViewMat, kInverseProjMat);

	float3 Albedo = pow( abs( AlbedoSample ), float3(2.2, 2.2, 2.2) );
	float3 LightLuminance = PBRLightPixel( AlbedoSample, NormalSample, RoughnessSample, MetallicSample, SpecularSample, WorldPos, Input.UVs );
	
    float3 Ambient = float3(0.03, 0.03, 0.03) * Albedo;
	float3 Color = Ambient + LightLuminance;
    Color = Color / (Color + float3(1, 1, 1));
    Color = pow(abs(Color), float3(1 / 2.2, 1 / 2.2, 1 / 2.2));
	Output.Result = float4(Color , 1.f);
	
	// DEBUG
	//
	//float LinearDepth = LinearizeDepth(Depth, kCameraNearZ, kCameraFarZ);
	//Output.Result = float4(AlbedoSample, 1);
	
	return Output;
}
