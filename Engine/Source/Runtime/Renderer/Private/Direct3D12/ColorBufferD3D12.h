#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IColorBuffer.h"
#include "PixelBufferD3D12.h"
#include "CommonEnums.h"

class FRenderDevice;


class RENDER_API FColorBufferD3D12 : public FColorBuffer, public FPixelBufferD3D12
{
public:
	FColorBufferD3D12();
	virtual ~FColorBufferD3D12();

	virtual void CreateFromSwapChain(FRenderDevice* pDevice, const TChar* Name, void* pResource) override;
	virtual void Create(FRenderDevice* pDevice, const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format) override;
	virtual void DestroyAPIResource() override;

	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const { return m_SRVHandle; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const { return m_RTVHandle; }

protected:
	virtual void CreateDerivedViews(FRenderDevice* pDevice, EFormat Format, uint32 ArraySize, uint32 NumMips) override;

protected:
	D3D12_CPU_DESCRIPTOR_HANDLE m_SRVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RTVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_UAVHandle[12];
};
