// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/Core.hlsli"
#include "ForwardShadingCommon.hlsli"


// Opaque material Textures
//
HE_DECLARE_TEXTURE2D(MaterialAlbedo, 0);

//
// Entry Point
//
FP_PSOutput main(FP_PSInput Input)
{
    FP_PSOutput Output;

	// Sample G-Buffer.
    float3 AlbedoSample = MaterialAlbedo.Sample(LinearWrapSampler, Input.UVs).rgb;
    Output.LitPixel = float4(AlbedoSample, 1.f);
    
    return Output;
}
