// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
    Common functions for physically-based rendering.
*/
#include "../Core/Common.hlsli"

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