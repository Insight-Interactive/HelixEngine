// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "DeferredShadingCommon.hlsli"


//
// Entry Point
//
GP_PSOutput main( GP_PSInput Input )
{
	GP_PSOutput Output;

	Output.Albedo = float4( 255.f, 255.f, 0.f, 1.f );
	Output.Normal = float4(0.f, 0.f, 0.f, 1.f);
	Output.Roughness = 0.f;
	Output.Metallic = 0.f;
	Output.Specular = 0.04f;
	//Output.Emissive = float3(0.f, 0.f, 0.f);

	return Output;
}
