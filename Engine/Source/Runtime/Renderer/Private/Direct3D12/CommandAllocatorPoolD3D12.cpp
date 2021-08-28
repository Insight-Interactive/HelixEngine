#include "RendererPCH.h"

#include "CommandAllocatorPoolD3D12.h"


D3D12CommandAllocatorPool::D3D12CommandAllocatorPool(const D3D12_COMMAND_LIST_TYPE& Type)
	: m_cCommandListType(Type)
	, m_pID3D12Device(NULL)
{
}

D3D12CommandAllocatorPool::~D3D12CommandAllocatorPool()
{
	m_pID3D12Device = NULL;
}

void D3D12CommandAllocatorPool::Initialize(ID3D12Device* pD3DDevice)
{
	HE_ASSERT(pD3DDevice != NULL);
	m_pID3D12Device = pD3DDevice;
}

ID3D12CommandAllocator* D3D12CommandAllocatorPool::RequestAllocator(uint64 CompletedFenceValue)
{
	m_AllocatorMutex.Enter();

	ID3D12CommandAllocator* pAllocator = NULL;

	if (!m_ReadyAllocators.empty())
	{
		std::pair<uint64, ID3D12CommandAllocator*>& AllocatorPair = m_ReadyAllocators.front();

		if (AllocatorPair.first <= CompletedFenceValue)
		{
			pAllocator = AllocatorPair.second;
			HRESULT hr = pAllocator->Reset();
			ThrowIfFailedMsg(hr, TEXT("Failed to reset command allocator!"));
			m_ReadyAllocators.pop();
		}
	}

	// If no allocator's were ready to be reused, create a new one
	if (pAllocator == nullptr)
	{
		HRESULT hr = m_pID3D12Device->CreateCommandAllocator(m_cCommandListType, IID_PPV_ARGS(&pAllocator));
		ThrowIfFailedMsg(hr, TEXT("Failed to create command allocator"));
		wchar_t AllocatorName[32];
		swprintf(AllocatorName, 32, L"CommandAllocator %zu", m_AllocatorPool.size());
		pAllocator->SetName(AllocatorName);
		m_AllocatorPool.push_back(pAllocator);
	}

	m_AllocatorMutex.Exit();
	return pAllocator;
}

void D3D12CommandAllocatorPool::DiscardAllocator(uint64 FenceValue, ID3D12CommandAllocator* pAllocator)
{
	m_AllocatorMutex.Enter();

	// That fence value indicates we are free to reset the allocator
	m_ReadyAllocators.push(std::make_pair(FenceValue, pAllocator));

	m_AllocatorMutex.Exit();
}