// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "../Core/PBRCommon.hlsli"
#include "ForwardShadingCommon.hlsli"

// Material params
//
HE_DECLARE_CONSTANT_BUFFER( WireframeParams, 3 )
{
    float3 kColor;
    float kOpacity = 1;
}

//
// Entry Point
//
FP_PSOutput main( FP_PSInput Input )
{
    FP_PSOutput Output;

    Output.LitPixel = float4(kColor, kOpacity);

    return Output;
}
