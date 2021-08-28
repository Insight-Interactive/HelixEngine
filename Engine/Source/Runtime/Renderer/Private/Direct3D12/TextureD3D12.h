#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ITexture.h"
#include "GpuResourceD3D12.h"
#include "DynamicDescriptorHeapD3D12.h"


class RENDER_API TextureD3D12 : public ITexture, public GpuResourceD3D12
{
public:
	TextureD3D12()
	{
		m_hCpuDescriptorHandle.ptr = HE_INVALID_GPU_ADDRESS;
	}
	virtual ~TextureD3D12() {}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_hCpuDescriptorHandle; }

	DescriptorHandle GetShaderVisibleDescriptorHandle() const { return m_DescriptorHandle; }

	virtual void Create2D(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitData) override;
	virtual void CreateCube(uint64 RowPitchBytes, uint64 Width, uint64 Height, EFormat Format, const void* InitialData) override;
	virtual void Destroy() override;

protected:
	void AssociateWithShaderVisibleHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE m_hCpuDescriptorHandle;
	DescriptorHandle m_DescriptorHandle;
};
