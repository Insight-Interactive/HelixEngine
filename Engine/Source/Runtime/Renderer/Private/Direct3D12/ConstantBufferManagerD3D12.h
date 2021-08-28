#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"
#include "IConstantBufferManager.h"
#include "GpuResourceD3D12.h"


class RENDER_API ConstantBufferD3D12 : public IConstantBuffer, public GpuResourceD3D12
{
	friend class ConstantBufferManagerD3D12;
	friend class CommandContextD3D12;
public:
	ConstantBufferD3D12()
		: m_CBV()
		, m_pWritePointer(NULL)
	{
	}
	~ConstantBufferD3D12() = default;

	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE CBV() const;
	FORCEINLINE void* GetGPUWritePointer() const;

protected:
	FORCEINLINE void UploadBuffer();
	FORCEINLINE static uint32 GetAlignedBufferSize(uint32 BufferSize);

	virtual void Create(const WChar* Name, uint32 BufferSize) override;

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_CBV;
	void* m_pWritePointer;
};


class RENDER_API ConstantBufferManagerD3D12 : public IConstantBufferManager
{
public:
	ConstantBufferManagerD3D12()
		: m_pID3D12DeviceRef(NULL)
	{
	}
	virtual ~ConstantBufferManagerD3D12()
	{
		m_pID3D12DeviceRef = NULL;
	}

	virtual void Initialize() override;


	virtual void CreateConstantBuffer(const WChar* Name, IConstantBuffer** OutBuffer, uint32 BufferSizeInBytes) override;
	virtual void DestroyConstantBuffer(ConstantBufferUID BufferHandle);

private:
	ID3D12Device* m_pID3D12DeviceRef;
	std::unordered_map<ConstantBufferUID, ConstantBufferD3D12> m_ConstantBufferLUT;
};

//
// Inline function implementations
//

// D3D12ConstantBuffer

FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE ConstantBufferD3D12::CBV() const
{
	return m_CBV;
}

FORCEINLINE void* ConstantBufferD3D12::GetGPUWritePointer() const
{
	return m_pWritePointer;
}

FORCEINLINE void ConstantBufferD3D12::UploadBuffer()
{
	memcpy(GetGPUWritePointer(), RCast<const void*>(m_Data), GetBufferSize());
}

FORCEINLINE uint32 ConstantBufferD3D12::GetAlignedBufferSize(uint32 BufferSize)
{
	return (BufferSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1); // Must be a multiple 256 bytes
}
