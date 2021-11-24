#include "../../Public/Core/Core.hlsli"


// Structures
//
struct SP_PSInput
{
    float4 Position : SV_POSITION;
    float3 UVs      : TEXCOORDS;
};

struct SP_PSOutput
{
    float4 FragColor : SV_TARGET;
};

// Textures
//
HE_DECLARE_TEXTURECUBE( SkyMap, 0);

//
// Entry Point
//
SP_PSOutput main(SP_PSInput Input)
{
    SP_PSOutput Output;
    
    float3 Color = SkyMap.Sample(LinearWrapSampler, Input.UVs).rgb;
    Output.FragColor = float4(Color, 1.f);
    
    return Output;
}
