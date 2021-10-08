#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"
#include "Containers/TDynamicArray.h"
#include "CriticalSection.h"
#include "../D3DCommon/D3DCommon.h"

class RENDER_API FCommandAllocatorPoolD3D12
{
public:
	FCommandAllocatorPoolD3D12(const D3D12_COMMAND_LIST_TYPE& Type);
	~FCommandAllocatorPoolD3D12();

	void Initialize(ID3D12Device* pD3DDevice);

	ID3D12CommandAllocator* RequestAllocator(uint64 CompletedFenceValue);
	void DiscardAllocator(uint64 FenceValue, ID3D12CommandAllocator* pAllocator);

	inline uint64 Size() const { return m_AllocatorPool.size(); }

protected:
	const D3D12_COMMAND_LIST_TYPE m_cCommandListType;
	ID3D12Device* m_pID3D12Device;
	std::vector<ID3D12CommandAllocator*> m_AllocatorPool;
	std::queue<std::pair<uint64, ID3D12CommandAllocator*>> m_ReadyAllocators;
	CriticalSection m_AllocatorMutex;
};
