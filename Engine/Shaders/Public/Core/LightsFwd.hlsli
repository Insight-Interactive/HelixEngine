
//
// Common Light Stuctures.
//

struct PointLight
{
    float3 Position;
    float Radius;
    float4 Color;
    float Brightness;
    uint Id;
};

struct DirectionalLight
{
    float4 Direction;
    float4 Color;
    float Brightness;
    uint Id;
};

