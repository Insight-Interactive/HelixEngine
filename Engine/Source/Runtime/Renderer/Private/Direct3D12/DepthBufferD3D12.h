#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IDepthBuffer.h"
#include "PixelBufferD3D12.h"


class RENDER_API DepthBufferD3D12 : public IDepthBuffer, public PixelBufferD3D12
{
public:
	DepthBufferD3D12(float ClearDepth = 1.0f, uint8 ClearStencil = 0)
		: IDepthBuffer(ClearDepth, ClearStencil)
	{
		m_hDSV[0].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[1].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[2].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[3].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDepthSRV.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hStencilSRV.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}
	virtual ~DepthBufferD3D12() {}

	virtual void Create(const WChar* Name, uint32 Width, uint32 Height, EFormat Format) override;

	// Get pre-created CPU-visible descriptor handles
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV() const { return m_hDSV[0]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_DepthReadOnly() const { return m_hDSV[1]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_StencilReadOnly() const { return m_hDSV[2]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_ReadOnly() const { return m_hDSV[3]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthSRV() const { return m_hDepthSRV; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetStencilSRV() const { return m_hStencilSRV; }

protected:

	void CreateDerivedViews(IDevice* Device, EFormat Format);

	D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV[4];
	D3D12_CPU_DESCRIPTOR_HANDLE m_hDepthSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hStencilSRV;
};
