// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "../../Public/Core/Core.hlsli"


struct VSInput
{
    float3 Position : POSITION;
    float2 TexCoords : UVS;
};

struct VSOutput
{
    float4 ScreenPosition : SV_POSITION;
    float2 TexCoords : UVS;
};

VSOutput main( VSInput Input )
{
    VSOutput Result;

    Result.ScreenPosition   = float4(Input.Position, 1.0);
    Result.TexCoords        = Input.TexCoords;

    return Result;
}

