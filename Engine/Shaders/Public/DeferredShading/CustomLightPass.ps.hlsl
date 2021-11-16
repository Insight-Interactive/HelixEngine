#include "../Core/Core.hlsli"
#include "../Core/Common.hlsli"
#include "DeferredShadingCommon.hlsli"


// Samplers
//
HE_DECLARE_SAMPLERSTATE( LinearWrapSampler, 0);

// GBuffer Inputs
//
HE_DECLARE_TEXTURE2D( SceneDepth      , 0);
HE_DECLARE_TEXTURE2D( AlbedoGBuffer   , 1);
HE_DECLARE_TEXTURE2D( NormalGBuffer   , 2);


//
// Entry Point
//
LP_PSOutput main(LP_PSInput Input)
{
	LP_PSOutput Output;

	// Sample G-Buffer.
	float3 AlbedoSample = AlbedoGBuffer.Sample(LinearWrapSampler, Input.UVs).rgb;
	float3 NormalSample = NormalGBuffer.Sample(LinearWrapSampler, Input.UVs).rgb;
    float Depth = SceneDepth.Sample(LinearWrapSampler, Input.UVs).r;
    float3 WorldPos = ConstructWorldPosFromDepth(Depth, Input.UVs, kInverseViewMat, kInverseProjMat);
	
	// Acummulate point light luminance.
    float3 PointLightLuminance = float3(0.f, 0.f, 0.f);
    for (uint i = 0; i < kNumPointLights; i++)
    {
        float3 LightDir = -normalize(kPointLights[i].Position.xyz - WorldPos);
        float Angle = max(dot(NormalSample, LightDir), 0);

        float Distance = length(kPointLights[i].Position.xyz - WorldPos);
        float Attenuation = 1.0f / (Distance * Distance);
        //float Attenuation = pow(saturate(1.0f - pow(Distance / kPointLights[i].Radius, 4)), 2) / pow(Distance, 2) + 1;
        PointLightLuminance += ((kPointLights[i].Color.rgb * kPointLights[i].Brightness) * Attenuation) * Angle;
    }
	
	// Accumulate directional light luminance.
    float3 DirectionalLightLuminance = float3(0.f, 0.f, 0.f);
    float3 LightDir = normalize(float3(0.f, 0.f, 0.25f));
    float Angle = max(dot(NormalSample, LightDir), 0);
    DirectionalLightLuminance += float3(1.f, 1.f, 1.f) * Angle;
    //for (uint d = 0; d < kNumDirectionalLights; d++)
    //{
    //    float3 LightDir = normalize(kDirectionalLights[d].Direction.xyz);
    //    float Angle = max(dot(NormalSample, LightDir), 0);
		
    //    DirectionalLightLuminance += (kDirectionalLights[d].Color.rgb * kDirectionalLights[d].Brightness) * Angle;
    //}
		
    float3 LightLuminance = PointLightLuminance + DirectionalLightLuminance;
	
    Output.Result = float4(AlbedoSample * LightLuminance, 1.f);
	
	// DEBUG
	//
    //float LinearDepth = LinearizeDepth(Depth, kCameraNearZ, kCameraFarZ);
    //Output.Result = float4(AlbedoSample, 1);
	
	return Output;
}
