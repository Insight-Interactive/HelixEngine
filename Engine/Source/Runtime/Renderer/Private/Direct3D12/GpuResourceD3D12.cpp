#include "RendererPCH.h"

#include "GpuResourceD3D12.h"
#include "../D3DCommon/D3DCommon.h"


FGpuResourceD3D12::FGpuResourceD3D12(ID3D12Resource* pResource, EResourceState CurrentState)
	: FGpuResource(CurrentState, RESOURCE_STATE_INVALID)
	, m_pID3D12Resource(pResource)
	, m_GpuVirtualAddress(HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL)
{
}

FGpuResourceD3D12::FGpuResourceD3D12()
	: m_pID3D12Resource(NULL)
	, m_GpuVirtualAddress(HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL)
{
}

FGpuResourceD3D12::~FGpuResourceD3D12()
{
	Destroy();
}

void FGpuResourceD3D12::Destroy()
{
	m_pID3D12Resource = nullptr;
	m_GpuVirtualAddress = HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	++m_VersionID;
}
