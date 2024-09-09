// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "PixelBuffer.h"

#include "Color.h"

class FRenderDevice;

class FColorBuffer : public FPixelBuffer
{
	friend class FSwapChain;
public:
	FColorBuffer();
	virtual ~FColorBuffer();

	void CreateFromSwapChain(const WChar* Name, void* pResource);
	void Create(const WChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format);
	void DestroyAPIResource();

	FORCEINLINE void SetClearColor(FColor Color) { m_ClearColor = Color; }
	FORCEINLINE FColor GetClearColor() const { return m_ClearColor; }

#if R_WITH_D3D12
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const { return m_SRVHandle; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const { return m_RTVHandle; }
#endif

protected:
	void CreateDerivedViews(FRenderDevice& Device, EFormat Format, uint32 ArraySize, uint32 NumMips);

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

protected:
#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE m_SRVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RTVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_UAVHandle[12];

#endif

};
