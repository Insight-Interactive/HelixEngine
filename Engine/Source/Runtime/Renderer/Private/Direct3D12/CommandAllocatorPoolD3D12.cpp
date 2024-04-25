#include "RendererPCH.h"
#if R_WITH_D3D12

#include "CommandAllocatorPool.h"

#include "RenderDevice.h"


FCommandAllocatorPool::FCommandAllocatorPool(const D3D12_COMMAND_LIST_TYPE& Type)
	: m_cCommandListType(Type)
	, m_pRenderDeviceRef(NULL)
{
}

FCommandAllocatorPool::~FCommandAllocatorPool()
{
	m_pRenderDeviceRef = NULL;
}

void FCommandAllocatorPool::Initialize(FRenderDevice& RenderDevice )
{
	m_pRenderDeviceRef = &RenderDevice;
}

ID3D12CommandAllocator* FCommandAllocatorPool::RequestAllocator(uint64 CompletedFenceValue)
{
	ID3D12Device* pID3D12Device = (ID3D12Device*)m_pRenderDeviceRef->GetNativeDevice();

	m_AllocatorMutex.Enter();

	ID3D12CommandAllocator* pAllocator = NULL;

	if (!m_ReadyAllocators.empty())
	{
		std::pair<uint64, ID3D12CommandAllocator*>& AllocatorPair = m_ReadyAllocators.front();

		if (AllocatorPair.first <= CompletedFenceValue)
		{
			pAllocator = AllocatorPair.second;
			HRESULT hr = pAllocator->Reset();
			ThrowIfFailedMsg(hr, "Failed to reset command allocator!");
			m_ReadyAllocators.pop();
		}
	}

	// If no allocator's were ready to be reused, create a new one
	if (pAllocator == nullptr)
	{
		HRESULT hr = pID3D12Device->CreateCommandAllocator(m_cCommandListType, IID_PPV_ARGS(&pAllocator));
		ThrowIfFailedMsg(hr, "Failed to create command allocator");
#if R_DEBUG_GPU_RESOURCES
		WChar AllocatorName[32];
		swprintf(AllocatorName, 32, L"CommandAllocator %zu", m_AllocatorPool.size());
		pAllocator->SetName(AllocatorName);
#endif // R_DEBUG_GPU_RESOURCES
		m_AllocatorPool.push_back(pAllocator);
	}

	m_AllocatorMutex.Exit();
	return pAllocator;
}

void FCommandAllocatorPool::DiscardAllocator(uint64 FenceValue, ID3D12CommandAllocator* pAllocator)
{
	m_AllocatorMutex.Enter();

	// That fence value indicates we are free to reset the allocator
	m_ReadyAllocators.push(std::make_pair(FenceValue, pAllocator));

	m_AllocatorMutex.Exit();
}

#endif // R_WITH_D3D12
