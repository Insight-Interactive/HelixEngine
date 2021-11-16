#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"
#include "RendererCore.h"

#include "GpuResource.h"

#include "DescriptorHeap.h"


enum EFormat;

class RENDER_API HTexture : public FGpuResource
{
public:
	HTexture()
		: m_Width(0)
		, m_Height(0)
		, m_Depth(0)
	{
#if R_WITH_D3D12
		m_hCpuDescriptorHandle.ptr = HE_INVALID_GPU_ADDRESS;
#endif
	}
	virtual ~HTexture() 
	{
	}
	void Create2D(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitData);
	void CreateCube(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitialData);

	void Destroy();

	FORCEINLINE uint32 GetWidth()	const { return m_Width; }
	FORCEINLINE uint32 GetHeight()	const { return m_Height; }
	FORCEINLINE uint32 GetDepth()	const { return m_Depth; }

	FDescriptorHandle GetShaderVisibleDescriptorHandle() const { return m_DescriptorHandle; }

#if R_WITH_D3D12
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_hCpuDescriptorHandle; }
#endif

protected:
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_Depth;

protected:
#if R_WITH_D3D12
	void AssociateWithShaderVisibleHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE m_hCpuDescriptorHandle;
	FDescriptorHandle m_DescriptorHandle;
#endif
};
