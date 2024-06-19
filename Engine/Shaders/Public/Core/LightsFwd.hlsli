// Copyright 2024 Insight Interactive. All Rights Reserved.

//
// Common Light Stuctures.
//

struct PointLight
{
    float3 Position;
    float Radius;
    float3 Color;
    float Brightness;
    uint Id;

    float PLPad[55];
};

struct DirectionalLight
{
    float4 Direction;
    float4 Color;
    float Brightness;
    uint Id;

    float DLPad[54];
};

