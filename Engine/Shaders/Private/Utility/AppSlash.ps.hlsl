#include "../../Public/Core/Core.hlsli"


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
