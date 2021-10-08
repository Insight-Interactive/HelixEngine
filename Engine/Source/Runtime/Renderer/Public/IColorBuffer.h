#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"
#include "CommonEnums.h"
#include "Color.h"


class FRenderDevice;

class RENDER_API FColorBuffer
{
	friend class FSwapChain;
public:
	virtual ~FColorBuffer() = default;

	virtual void CreateFromSwapChain(FRenderDevice* pDevice, const TChar* Name, void* pResource) = 0;
	virtual void Create(FRenderDevice* pDevice, const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format) = 0;
	virtual void DestroyAPIResource() = 0;

	FORCEINLINE void SetClearColor(FColor Color) { m_ClearColor = Color; }
	FORCEINLINE FColor GetClearColor() const { return m_ClearColor; }

	template <typename T>
	inline T As()
	{
		return DCast<T>(this);
	}

protected:
	FColorBuffer()
		: m_ClearColor(0.f, 0.f, 0.f, 1.f)
		, m_NumMipMaps(0u)
		, m_FragmentCount(1u)
		, m_SampleCount(1u)
	{
	}

	virtual void CreateDerivedViews(FRenderDevice* pDevice, EFormat Format, uint32 ArraySize, uint32 NumMips) = 0;

	EResourceFlags CombineResourceFlags() const
	{
		EResourceFlags Flags = RF_None;

		if (Flags == RF_None && m_FragmentCount == 1)
			Flags |= RF_AllowUnorderedAccess;

		return RF_AllowRenderTarget | Flags;
	}

protected:
	FColor m_ClearColor;
	uint32 m_NumMipMaps;
	uint32 m_FragmentCount;
	uint32 m_SampleCount;
};
