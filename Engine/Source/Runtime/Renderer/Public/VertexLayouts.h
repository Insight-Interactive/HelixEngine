#pragma once

#include "MathCore.h"

struct FVertex3D
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 BiTangent;
	FVector4 Color;
	FVector2 UV0;
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
