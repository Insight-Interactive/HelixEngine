// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "MathCore.h"
#include "RendererCore.h"

struct FStaticVertex3D
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector2 UV0;
};

struct FSkinnedVertex3D
{
	FSkinnedVertex3D()
	{
		ZeroMemory( this, sizeof( FSkinnedVertex3D ) );
	}

	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector4 Color;
	FVector2 UV0;
	uint32 Joints[R_MAX_JOINTS_PER_VERTEX]; // Note: Keep this in sync with GP_VSInputSkinned in DeferedShadingCommon.hlsli
	float Weights[R_MAX_JOINTS_PER_VERTEX];
};

struct FVertex2D
{
	FVector2 Position;
	FVector2 UVs;
};

struct FSimpleVertex3D
{
	FVector3 Position;
};

struct FTextVertex2D
{
	FTextVertex2D( FVector4& _Position, FVector4& _UVs, FVector4& _Color )
		: Position( _Position ), UVs( _UVs ), Color( _Color ) {}

	FTextVertex2D( float r, float g, float b, float a, float u, float v, float tw, float th, float x, float y, float w, float h ) : Color( r, g, b, a ), UVs( u, v, tw, th ), Position( x, y, w, h ) {}
	FVector4 Position;
	FVector4 UVs;
	FVector4 Color;
};
