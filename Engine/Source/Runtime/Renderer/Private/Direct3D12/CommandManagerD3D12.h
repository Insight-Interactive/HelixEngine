#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ICommandManager.h"
#include "../D3DCommon/D3DCommon.h"
#include "CommandAllocatorPoolD3D12.h"

class FDeviceD3D12;

class RENDER_API FCommandQueueD3D12 final : public FCommandQueue
{
	friend FCommandManagerD3D12;
public:
	inline virtual void* GetNativeQueue() override { return RCast<void*>(m_pID3D12CommandQueue); }

	virtual void WaitForFence(uint64 FenceValue) override;
	virtual bool IsFenceCompleted(uint64 FenceValue) override;
	virtual uint64 IncrementFence() override;

	uint64 ExecuteCommandList(ID3D12CommandList* pCommandList);

	inline void DiscardAllocator(uint64 FenceValue, ID3D12CommandAllocator* pCommandAllocator)
	{
		m_D3D12AllocatorPool.DiscardAllocator(FenceValue, pCommandAllocator);
	}

	inline ID3D12CommandAllocator* RequestAllocator()
	{
		uint64 CompletedValue = m_pFence->GetCompletedValue();
		return m_D3D12AllocatorPool.RequestAllocator(CompletedValue);
	}


private:
	FCommandQueueD3D12(const ECommandListType Type);
	virtual ~FCommandQueueD3D12();

	void Initialize(ID3D12Device* pDevice);

	//
	// D3D Initializations
	//
	void CreateD3D12Queue();
	void CreateSyncObjects();


	ID3D12Device* m_pID3DDeviceRef;

	ID3D12CommandQueue* m_pID3D12CommandQueue;
	FCommandAllocatorPoolD3D12	m_D3D12AllocatorPool;

	// Sync
	//
	CriticalSection	m_FenceMutex;
	CriticalSection	m_EventMutex;
	ID3D12Fence*	m_pFence;
	uint64			m_NextFenceValue;
	uint64			m_LastCompletedFenceValue;
	HANDLE			m_FenceEventHandle;
};


class RENDER_API FCommandManagerD3D12 final : public FCommandManager
{
	friend class FRenderContextFactory;
	friend class D3D12RenderContextFactory;
public:
	virtual void Initialize(FRenderDevice* pDevice) override;
	virtual void CreateNewCommandContext(const ECommandListType& Type, FCommandContext** pContext, void** pCommandAllocator) override;

	virtual void WaitForFence(uint64 FenceValue) override;

	FCommandManagerD3D12()
		: m_pID3D12DeviceRef(NULL)
		, m_pD3D12DeviceRef(NULL)
		, m_D3D12GraphicsQueue(CLT_Direct)
		, m_D3D12ComputeQueue(CLT_Compute)
	{
	}
	virtual ~FCommandManagerD3D12()
	{
		m_pID3D12DeviceRef = NULL;
		m_pD3D12DeviceRef = NULL;
	}

protected:

	virtual void UnInitialize() override;

	// References
	ID3D12Device* m_pID3D12DeviceRef;
	FDeviceD3D12* m_pD3D12DeviceRef;


	FCommandQueueD3D12 m_D3D12GraphicsQueue;
	FCommandQueueD3D12 m_D3D12ComputeQueue;

};
