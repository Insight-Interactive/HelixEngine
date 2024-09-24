// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

class FColor
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
	FORCEINLINE FColor( float rgba )
		: FColor(rgba, rgba, rgba, rgba )
	{
	}
	FORCEINLINE ~FColor()
	{
	}

	FORCEINLINE FVector4 ToVector4() const
	{
		return FVector4( R, G, B, A );
	}

	FORCEINLINE FVector3 ToVector3() const
	{
		return FVector3(R, G, B);
	}

	float R, G, B, A;

public: // statics
	static FColor BlackOpaque;
	static FColor WhiteOpaque;
	static FColor RedOpaque;
	static FColor GreenOpaque;
	static FColor BlueOpaque;
	static FColor PurpleOpaque;
};

