#include "../../Public/Core/Core.hlsli"


// Samplers
//
SamplerState g_LinearWrapSampler : register(s0);

// Textures
//
Texture2D g_SplashImage : register(t0);

// Structures
//
struct PSInput
{
    float4 ScreenPosition : SV_POSITION;
    float2 TexCoords : UVS;
};

struct PSOutput
{
    float4 Color : SV_TARGET;
};

//
// Entry Point
//
PSOutput main( PSInput Input )
{
    PSOutput Result;

    Result.Color = g_SplashImage.Sample( g_LinearWrapSampler, Input.TexCoords );

    return Result;
}
