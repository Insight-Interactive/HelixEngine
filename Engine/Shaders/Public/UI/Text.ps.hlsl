// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "UIShadingCommon.hlsli"

HE_DECLARE_TEXTURE2D( TextGlyph, 0 );

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
    float2 UVs      : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 Color : SV_TARGET;
};

PS_OUTPUT main( VS_OUTPUT Input )
{
    PS_OUTPUT Result;
    
    float3 CharacterColor = Input.Color.rgb;
    float CharacterAlpha = Input.Color.a * TextGlyph.Sample( LinearWrapSampler, Input.UVs ).a;
    
    Result.Color = float4(CharacterColor, CharacterAlpha );
    return Result;
}
