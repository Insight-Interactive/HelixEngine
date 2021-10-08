#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

class RENDER_API FColor
{
public:
	FORCEINLINE FColor()
		: R(0.f)
		, G(0.f)
		, B(0.f)
		, A(0.f)
	{
	}
	FORCEINLINE FColor(float r, float g, float b, float a)
		: R(r)
		, G(g)
		, B(b)
		, A(a)
	{
	}
	FORCEINLINE FColor(float r, float g, float b)
		: R(r)
		, G(g)
		, B(b)
		, A(1.f)
	{
	}
	FORCEINLINE ~FColor()
	{
	}

	FORCEINLINE FVector4 ToVector4() const
	{
		return FVector4( R, G, B, A );
	}


	float R, G, B, A;
};

