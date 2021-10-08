/*
	File: HTexture2D.h
	Source: None
	Author: Garrett Courtney

	Description:
	Client facing reference counted 2D texture.

*/
#pragma once

#include "ITextureManager.h"
#include "ITexture.h"

class HTexture2D : public HTextureRef
{
public:
	HTexture2D()
	{
	}
	virtual ~HTexture2D()
	{
	}

	uint32 GetWidth() const;
	uint32 GetHeight() const;

};

//
// Inline function implementations
//

FORCEINLINE uint32 HTexture2D::GetWidth() const
{
	return Get()->GetWidth();
}

FORCEINLINE uint32 HTexture2D::GetHeight() const
{
	return Get()->GetHeight();
}
