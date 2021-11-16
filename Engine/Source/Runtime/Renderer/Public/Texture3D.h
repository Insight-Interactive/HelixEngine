/*
	File: HTexture2D.h
	Source: None
	Author: Garrett Courtney

	Description:
	Client facing reference counted 3D texture.

*/
#pragma once

#include "TextureManager.h"
#include "Texture.h"

class HTexture3D : public HTextureRef
{
public:
	HTexture3D()
	{
	}
	virtual ~HTexture3D()
	{
	}

	uint32 GetWidth() const;
	uint32 GetHeight() const;
	uint32 GetDepth() const;

};

//
// Inline function implementations
//

FORCEINLINE uint32 HTexture3D::GetWidth() const
{
	return Get()->GetWidth();
}

FORCEINLINE uint32 HTexture3D::GetHeight() const
{
	return Get()->GetHeight();
}

FORCEINLINE uint32 HTexture3D::GetDepth() const
{
	return Get()->GetDepth();
}
