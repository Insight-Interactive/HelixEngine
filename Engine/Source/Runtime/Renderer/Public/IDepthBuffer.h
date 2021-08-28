#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

enum EFormat;

class RENDER_API IDepthBuffer
{
public:
	virtual ~IDepthBuffer() {}

	virtual void Create(const WChar* Name, uint32 Width, uint32 Height, EFormat Format) = 0;

	FORCEINLINE float GetClearDepth() const { return m_ClearDepth; }
	FORCEINLINE uint8 GetClearStencil() const { return m_ClearStencil; }

	template <typename T>
	inline T As()
	{
		return DCast<T>( this );
	}

protected:
	IDepthBuffer(float ClearDepth, uint8 ClearStencil)
		: m_ClearDepth(ClearDepth)
		, m_ClearStencil(ClearStencil)
	{
	}

	float m_ClearDepth;
	uint8 m_ClearStencil;
};
