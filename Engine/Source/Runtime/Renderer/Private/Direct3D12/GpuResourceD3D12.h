#pragma once

#include "RendererFwd.h"

#include "IGpuResource.h"

#include "CommonEnums.h"

class RENDER_API FGpuResourceD3D12 : public FGpuResource
{
public:
	FGpuResourceD3D12(ID3D12Resource* pResource, EResourceState CurrentState);
	FGpuResourceD3D12();
	virtual ~FGpuResourceD3D12();

	virtual void Destroy() override;
	inline void DestroyCOMResource() { HE_COM_SAFE_RELEASE(m_pID3D12Resource); }

	//
	// Getters/Setters
	//
	ID3D12Resource* GetResource() const { return m_pID3D12Resource.Get(); }
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return m_GpuVirtualAddress; }

	ID3D12Resource* operator -> () { return m_pID3D12Resource.Get(); }
	const ID3D12Resource* operator->() const { return m_pID3D12Resource.Get(); }
	ID3D12Resource** GetAddressOf() { return m_pID3D12Resource.GetAddressOf(); }

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pID3D12Resource;
	D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;
};
