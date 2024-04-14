#pragma once

#include "MathCore.h"

struct FVertex3D
{
	FVertex3D()
	{
		HE_ZERO_MEMORY( this, sizeof( FVertex3D ) );
	}
	
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector4 Color;
	FVector2 UV0;
};

struct FSkinnedVertex3D
{
	FSkinnedVertex3D()
	{
		HE_ZERO_MEMORY( this, sizeof( FSkinnedVertex3D) );
	}
	
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector4 Color;
	FVector2 UV0;
	uint8 Joints[4];// Note: Keep this in sync with Renderer::SkeletalMesh.h
	float Weights[4];
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
