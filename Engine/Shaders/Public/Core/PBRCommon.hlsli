// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
    Common functions for physically-based rendering.
*/
#include "../Core/Common.hlsli"
#include "../Core/Core.hlsli"

float DistributionGGX(float3 N, float3 H, float Roughness)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float Roughness)
{
    float r = (Roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float Roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, Roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, Roughness);
	
    return ggx1 * ggx2;
}

float3 FresnelSchlick(float CosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - CosTheta, 0.0f, 1.0f), 5.0f);
}

float3 PBRLightPixel( float3 AlbedoSample, float3 NormalSample, float RoughnessSample, float MetallicSample, float SpecularSample, float3 WorldPos, float2 PixelUVs )
{
    float3 F0 = float3(SpecularSample, SpecularSample, SpecularSample);
    F0 = lerp( F0, AlbedoSample, MetallicSample );
    float3 V = normalize( kCameraPos - WorldPos );

    // Accumulate point light luminance.
    //
    float3 PointLightLuminance = float3(0.f, 0.f, 0.f);
    for (uint i = 0; i < kNumPointLights; i++)
    {
        float3 L = normalize( kPointLights[i].Position - WorldPos );
        float3 H = normalize( V + L );

        float Distance = length( kPointLights[i].Position - WorldPos );
        float Attenuation = 1.0f / (Distance * Distance);
        float3 Radiance = kPointLights[i].Color * Attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX( NormalSample, H, RoughnessSample );
        float G = GeometrySmith( NormalSample, V, L, RoughnessSample );
        float3 F = FresnelSchlick( max( dot( H, V ), 0.0f ), F0 );

        float3 kS = F;
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - MetallicSample;

        float3 Numerator = NDF * G * F;
        float Denominator = 4.0 * max( dot( NormalSample, V ), 0.0 ) * max( dot( NormalSample, L ), 0.0 ) + 0.0001;
        float3 Specular = Numerator / Denominator;

        float NdotL = max( dot( NormalSample, L ), 0.0 );
        PointLightLuminance += ((kD * AlbedoSample / PI + Specular) * Radiance * NdotL) * kPointLights[i].Brightness;
    }

    // Accumulate directional light luminance.
    //
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

    return float3(PointLightLuminance);
}
