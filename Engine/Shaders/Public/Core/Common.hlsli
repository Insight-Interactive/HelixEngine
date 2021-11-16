
/*
    Puts a value back into normalized device coordinates.
*/
#define ToNDC(Value) Value * 2 - 1


float3 ConstructWorldPosFromDepth(float Depth, float2 TexCoords, float4x4 InverseViewMatrix, float4x4 InverseProjectionMatrix)
{
    float Z = (Depth);
        
    float2 ClipSpaceCoords;
    ClipSpaceCoords.x = (TexCoords.x) * 2 - 1;
    ClipSpaceCoords.y = (1 - TexCoords.y) * 2 - 1;
    
    float4 ClipSpacePos = float4(ClipSpaceCoords, Z, 1.f);
    float4 ViewSpacePos = mul(ClipSpacePos, InverseProjectionMatrix);

    ViewSpacePos /= ViewSpacePos.w;

    float4 WorldSpacePos = mul(ViewSpacePos, InverseViewMatrix);

    return WorldSpacePos.xyz;
}

float LinearizeDepth(float Depth, float CameraNearZ, float CameraFarZ)
{
    float z = ToNDC(Depth);
    return (2.f * CameraNearZ * CameraFarZ) / (CameraFarZ + CameraNearZ - z * (CameraFarZ - CameraNearZ)) / CameraFarZ;
}
