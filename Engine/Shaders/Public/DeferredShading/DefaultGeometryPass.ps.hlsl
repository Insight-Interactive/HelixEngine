#include "../Core/Core.hlsli"
#include "DeferredShadingCommon.hlsli"


// Samplers
//
HE_DECLARE_SAMPLERSTATE(LinearWrapSampler, 0);

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

	const float3x3 TangentToView = float3x3(normalize(Input.Tangent),
											normalize(Input.BiTangent),
													  Input.Normal);
	float3 NormalTangentSpace;
	NormalTangentSpace.x =  NormalSample.x * 2.0f - 1.0f;
	NormalTangentSpace.y = -NormalSample.y * 2.0f + 1.0f;
	NormalTangentSpace.z =  NormalSample.z;
	NormalTangentSpace = normalize(mul(NormalTangentSpace, TangentToView));

	Output.Normal = float4(NormalTangentSpace, 1);
    Output.Albedo = float4(AlbedoSample, 1) + kMaterialColor;
	
	return Output;
}