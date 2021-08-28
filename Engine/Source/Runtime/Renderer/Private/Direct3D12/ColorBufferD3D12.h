#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IColorBuffer.h"
#include "PixelBufferD3D12.h"
#include "CommonEnums.h"

class IDevice;


class RENDER_API ColorBufferD3D12 : public IColorBuffer, public PixelBufferD3D12
{
public:
	ColorBufferD3D12();
	virtual ~ColorBufferD3D12();

	virtual void CreateFromSwapChain(IDevice* pDevice, const TChar* Name, void* pResource) override;
	virtual void Create(IDevice* pDevice, const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format) override;
	virtual void DestroyAPIResource() override;

	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const { return m_SRVHandle; }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const { return m_RTVHandle; }

protected:
	virtual void CreateDerivedViews(IDevice* pDevice, EFormat Format, uint32 ArraySize, uint32 NumMips) override;

protected:
	D3D12_CPU_DESCRIPTOR_HANDLE m_SRVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_RTVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_UAVHandle[12];
};
