#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

class RENDER_API Color
{
public:
	FORCEINLINE Color()
		: R(0.f)
		, G(0.f)
		, B(0.f)
		, A(0.f)
	{
	}
	FORCEINLINE Color(float r, float g, float b, float a)
		: R(r)
		, G(g)
		, B(b)
		, A(a)
	{
	}
	FORCEINLINE Color(float r, float g, float b)
		: R(r)
		, G(g)
		, B(b)
		, A(1.f)
	{
	}
	FORCEINLINE ~Color()
	{
	}

	float R, G, B, A;
};
