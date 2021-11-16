#include "../../Public/Core/Core.hlsli"


// Samplers
//
HE_DECLARE_SAMPLERSTATE(LinearWrapSampler, 0);

// Textures
//
HE_DECLARE_TEXTURE2D( SplashImage, 0);

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

    Result.Color = SplashImage.Sample( LinearWrapSampler, Input.TexCoords );

    return Result;
}
