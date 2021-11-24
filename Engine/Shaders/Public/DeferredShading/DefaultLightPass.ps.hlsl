// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
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

	
    float3 Albedo = pow(abs(AlbedoSample), float3(2.2, 2.2, 2.2));
	float3 Normal = (NormalSample);
    float3 F0 = float3(SpecularSample, SpecularSample, SpecularSample);
	F0 = lerp(F0, Albedo, MetallicSample);
    float3 ViewDir = normalize(kCameraPos - WorldPos);

    //Output.Result = float4(Normal, 1.f);
    //return Output;
	
	// Acummulate point light luminance.
	float3 PointLightLuminance = float3(0.f, 0.f, 0.f);
	for (uint i = 0; i < kNumPointLights; i++)
	{
        float3 LightDir = kPointLights[i].Position - WorldPos;
        float3 UnitLightDir = normalize(LightDir);
        float3 Halfway = normalize(ViewDir + UnitLightDir);
		
        //float Angle = max(dot(NormalSample, LightDir), 0);
        //PointLightLuminance += kPointLights[i].Color * Angle;
		
        float Distance = length(LightDir);
        float Attenuation = 1.0f / (Distance * Distance);
        float3 Radiance = kPointLights[i].Color * Attenuation;
		
        float3 F = FresnelSchlick(max(dot(Halfway, ViewDir), 0.0f), F0);
        float NDF = DistributionGGX(Normal, Halfway, RoughnessSample);
        float G = GeometrySmith(Normal, ViewDir, UnitLightDir, RoughnessSample);
		
        float3 Numerator = NDF * G * F;
        float Denominator = 4.0 * max(dot(Normal, ViewDir), 0.0) * max(dot(Normal, UnitLightDir), 0.0) + 0.0001;
        float3 Specular = Numerator / Denominator;
		
        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - MetallicSample;
		
        float NdotL = max(dot(Normal, UnitLightDir), 0.0);
        PointLightLuminance += (kD * Albedo / PI + Specular) * Radiance * NdotL;
    }
	
	// Accumulate directional light luminance.
	//float3 DirectionalLightLuminance = float3(0.f, 0.f, 0.f);
	//float3 LightDir = normalize(float3(0.f, 0.f, 0.25f));
	//float Angle = max(dot(Normal, LightDir), 0);
	//DirectionalLightLuminance += float3(1.f, 1.f, 1.f) * Angle;
	//for (uint d = 0; d < kNumDirectionalLights; d++)
	//{
	//    float3 LightDir = normalize(kDirectionalLights[d].Direction.xyz);
	//    float Angle = max(dot(NormalSample, LightDir), 0);
		
	//    DirectionalLightLuminance += (kDirectionalLights[d].Color.rgb * kDirectionalLights[d].Brightness) * Angle;
	//}
		
    float3 Ambient = float3(0.03, 0.03, 0.03) * Albedo;
	float3 Color = Ambient + PointLightLuminance /*+ DirectionalLightLuminance*/;
    Color = Color / (Color + float3(1, 1, 1));
    Color = pow(abs(Color), float3(1 / 2.2, 1 / 2.2, 1 / 2.2));
	Output.Result = float4(Color , 1.f);
	
	// DEBUG
	//
	//float LinearDepth = LinearizeDepth(Depth, kCameraNearZ, kCameraFarZ);
	//Output.Result = float4(AlbedoSample, 1);
	
	return Output;
}
