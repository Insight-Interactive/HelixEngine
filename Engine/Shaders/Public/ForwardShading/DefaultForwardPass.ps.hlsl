#include "../Core/Core.hlsli"
#include "ForwardShadingCommon.hlsli"


// Samplers
//
HE_DECLARE_SAMPLERSTATE( LinearWrapSampler, 0);

// Opaque material Textures
//
HE_DECLARE_TEXTURE2D( MaterialAlbedo, 0);
HE_DECLARE_TEXTURE2D( MaterialNormal, 1);

//
// Entry Point
//
FP_PSOutput main( FP_PSInput Input )
{
	FP_PSOutput Output;
	//Output.LitPixel = float4(0.50, 0.0, 0.0, 0.25);
	//return Output;

	// Sample G-Buffer.
	float3 AlbedoSample = MaterialAlbedo.Sample( LinearWrapSampler, Input.UVs ).rgb;
	float3 NormalSample = MaterialAlbedo.Sample( LinearWrapSampler, Input.UVs ).rgb;
    float3 WorldPos = Input.WorldPos; 
    Output.LitPixel = float4(AlbedoSample, .25);
	return Output;

	// Acummulate point light luminance.
	float3 PointLightLuminance = float3(0.f, 0.f, 0.f);
	for (uint i = 0; i < kNumPointLights; i++)
	{
		float3 LightDir = -normalize( kPointLights[i].Position.xyz - WorldPos );
		float Angle = max( dot( NormalSample, LightDir ), 0 );

		float Distance = length( kPointLights[i].Position.xyz - WorldPos );
		float Attenuation = 1.0f / (Distance * Distance);
		PointLightLuminance += ((kPointLights[i].Color.rgb * kPointLights[i].Brightness) * Attenuation) * Angle;
	}

	// Accumulate directional light luminance.
	float3 DirectionalLightLuminance = float3(0.f, 0.f, 0.f);
	for (uint d = 0; d < kNumDirectionalLights; d++)
	{
		float3 LightDir = normalize( kDirectionalLights[d].Direction.xyz );
		float Angle = max( dot( NormalSample, LightDir ), 0 );

		DirectionalLightLuminance += (kDirectionalLights[d].Color.rgb * kDirectionalLights[d].Brightness) * Angle;
	}

	float3 LightLuminance = PointLightLuminance + DirectionalLightLuminance;

	Output.LitPixel = float4(AlbedoSample * LightLuminance, 1.f);

	return Output;
}
