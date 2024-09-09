// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"
#include "Containers/TDynamicArray.h"
#include "CriticalSection.h"


class FRenderDevice;

class FCommandAllocatorPool
{
public:
	FCommandAllocatorPool(const D3D12_COMMAND_LIST_TYPE& Type);
	~FCommandAllocatorPool();

	void Initialize(FRenderDevice& RenderDevice );

	ID3D12CommandAllocator* RequestAllocator(uint64 CompletedFenceValue);
	void DiscardAllocator(uint64 FenceValue, ID3D12CommandAllocator* pAllocator);

	inline uint64 Size() const { return m_AllocatorPool.size(); }

protected:
	FRenderDevice* m_pRenderDeviceRef;

protected:
#if R_WITH_D3D12
	const D3D12_COMMAND_LIST_TYPE m_cCommandListType;
	std::vector<ID3D12CommandAllocator*> m_AllocatorPool;
	std::queue<std::pair<uint64, ID3D12CommandAllocator*>> m_ReadyAllocators;
	CriticalSection m_AllocatorMutex;
#endif

};
