#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

enum EFormat;

class RENDER_API HTexture
{
public:
	virtual void Create2D(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitData) = 0;
	virtual void CreateCube(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitialData) = 0;

	virtual void Destroy() = 0;

	FORCEINLINE uint32 GetWidth()	const { return m_Width; }
	FORCEINLINE uint32 GetHeight()	const { return m_Height; }
	FORCEINLINE uint32 GetDepth()	const { return m_Depth; }

protected:
	HTexture()
		: m_Width(0)
		, m_Height(0)
		, m_Depth(0)
	{
	}
	virtual ~HTexture() {}

	uint32 m_Width;
	uint32 m_Height;
	uint32 m_Depth;
};
