#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ICommandManager.h"
#include "../D3DCommon/D3DCommon.h"
#include "CommandAllocatorPoolD3D12.h"


class D3D12Device;
class D3D12CommandManager;

class RENDER_API CommandQueueD3D12 final : public ICommandQueue
{
	friend CommandManagerD3D12;
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
	CommandQueueD3D12(const ECommandListType Type);
	virtual ~CommandQueueD3D12();

	void Initialize(ID3D12Device* pDevice);

	//
	// D3D Initializations
	//
	void CreateD3D12Queue();
	void CreateSyncObjects();


	ID3D12Device* m_pID3DDeviceRef;

	ID3D12CommandQueue* m_pID3D12CommandQueue;
	D3D12CommandAllocatorPool	m_D3D12AllocatorPool;

	// Sync
	//
	CriticalSection	m_FenceMutex;
	CriticalSection	m_EventMutex;
	ID3D12Fence*	m_pFence;
	uint64			m_NextFenceValue;
	uint64			m_LastCompletedFenceValue;
	HANDLE			m_FenceEventHandle;
};


class RENDER_API CommandManagerD3D12 final : public ICommandManager
{
	friend class IRenderContextFactory;
	friend class D3D12RenderContextFactory;
public:
	virtual void Initialize(IDevice* pDevice) override;
	virtual void CreateNewCommandContext(const ECommandListType& Type, ICommandContext** pContext, void** pCommandAllocator) override;

	virtual void WaitForFence(uint64 FenceValue) override;

	CommandManagerD3D12()
		: m_pID3D12DeviceRef(NULL)
		, m_pD3D12DeviceRef(NULL)
		, m_D3D12GraphicsQueue(CLT_Direct)
		, m_D3D12ComputeQueue(CLT_Compute)
	{
	}
	virtual ~CommandManagerD3D12()
	{
		m_pID3D12DeviceRef = NULL;
		m_pD3D12DeviceRef = NULL;
	}

protected:

	virtual void UnInitialize() override;

	// References
	ID3D12Device* m_pID3D12DeviceRef;
	D3D12Device* m_pD3D12DeviceRef;


	CommandQueueD3D12 m_D3D12GraphicsQueue;
	CommandQueueD3D12 m_D3D12ComputeQueue;

};
