#pragma once

#include "MathCore.h"

struct Vertex3D
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector4 Color;
	FVector2 UV0;
};

struct Vertex2D
{
	FVector2 Position;
	FVector2 UVs;
};

struct SimpleVertex3D
{
	FVector3 Position;
};
