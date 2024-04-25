#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "PixelBuffer.h"


enum EFormat;

class RENDER_API FDepthBuffer : public FPixelBuffer
{
	friend class FCommandContext;
public:
	FDepthBuffer( float ClearDepth, uint8 ClearStencil )
		: m_ClearDepth( ClearDepth )
		, m_ClearStencil( ClearStencil )
	{
#if R_WITH_D3D12
		m_hDSV[0].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[1].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[2].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDSV[3].ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hDepthSRV.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_hStencilSRV.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
#endif
	}
	virtual ~FDepthBuffer() 
	{
	}

	void Create(const WChar* Name, uint32 Width, uint32 Height, EFormat Format);

	FORCEINLINE float GetClearDepth() const { return m_ClearDepth; }
	FORCEINLINE uint8 GetClearStencil() const { return m_ClearStencil; }

protected:
	void CreateDerivedViews( FRenderDevice& Device, EFormat Format );

protected:
	float m_ClearDepth;
	uint8 m_ClearStencil;

protected:
#if R_WITH_D3D12
	// Get pre-created CPU-visible descriptor handles.
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV()					const { return m_hDSV[0]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_DepthReadOnly()	const { return m_hDSV[1]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_StencilReadOnly() const { return m_hDSV[2]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_ReadOnly()		const { return m_hDSV[3]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthSRV()			const { return m_hDepthSRV; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetStencilSRV()			const { return m_hStencilSRV; }

	D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV[4];
	D3D12_CPU_DESCRIPTOR_HANDLE m_hDepthSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hStencilSRV;
	
#endif

};
