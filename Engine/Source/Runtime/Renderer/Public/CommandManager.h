#pragma once

#include "RendererFwd.h"

#include "CommandAllocatorPool.h"
#include "CommonEnums.h"


class FRenderDevice;
class FCommandContext;
class ICommandAllocator;


class RENDER_API FCommandQueue
{
	friend class FCommandManager;
public:
	FCommandQueue( const ECommandListType& Type );
	~FCommandQueue();

	void* GetNativeQueue();

	void WaitForFence(uint64 FenceValue);
	bool IsFenceCompleted(uint64 FenceValue);
	uint64 IncrementFence();
	void WaitForIdle() { WaitForFence(IncrementFence()); }


	uint64 ExecuteCommandList( void* pNativeCommandList );
	void DiscardAllocator( uint64 FenceValue, void* pNativeCommandAllocator );
	void* RequestAllocator();

protected:
	void InitializeAPI( FRenderDevice& pDevice );

	ECommandListType m_Type;

private:
#if R_WITH_D3D12

	// D3D Initializations
	//
	void CreateD3D12Queue();
	void CreateSyncObjects();


	FRenderDevice* m_pRenderDeviceRef;
	ID3D12CommandQueue* m_pID3D12CommandQueue;
	FCommandAllocatorPool	m_D3D12AllocatorPool;

	// Sync
	//
	CriticalSection	m_FenceMutex;
	CriticalSection	m_EventMutex;
	ID3D12Fence*	m_pFence;
	uint64			m_NextFenceValue;
	uint64			m_LastCompletedFenceValue;
	HANDLE			m_FenceEventHandle;
#endif

};

class RENDER_API FCommandManager
{
	friend class FRenderContextFactory;
	friend class FCommandManager;
	friend class FRenderContext;
public:
	FCommandManager()
		: m_pDeviceRef( nullptr )
		, m_GraphicsQueue( CLT_Direct )
		, m_ComputeQueue( CLT_Compute )
	{
	}
	virtual ~FCommandManager()
	{
		UnInitialize();
	}

	void Initialize(FRenderDevice& pDevice);
	void UnInitialize();

	void CreateNewCommandContext(const ECommandListType& Type, FCommandContext** pContext, void** pData);
	void WaitForFence(uint64 Value);
	void IdleGpu();
	FCommandQueue& GetQueue( const ECommandListType& Type );
	FCommandQueue& GetGraphicsQueue();
	FCommandQueue& GetComputeQueue();

	// Test to see if a fence has already been reached
	bool IsFenceComplete( uint64 FenceValue );

protected:
	FRenderDevice* m_pDeviceRef;

	FCommandQueue m_GraphicsQueue;
	FCommandQueue m_ComputeQueue;
};


//
// Inline function implementations
//

// FCommandQueue
//

FORCEINLINE void* FCommandQueue::GetNativeQueue()
{
#if R_WITH_D3D12
	return (void*)m_pID3D12CommandQueue;
#endif
	return NULL;
}

FORCEINLINE void FCommandQueue::DiscardAllocator( uint64 FenceValue, void* pNativeCommandAllocator )
{
#if R_WITH_D3D12
	m_D3D12AllocatorPool.DiscardAllocator( FenceValue, (ID3D12CommandAllocator*)pNativeCommandAllocator );
#endif
}
FORCEINLINE void* FCommandQueue::RequestAllocator()
{
#if R_WITH_D3D12
	uint64 CompletedValue = m_pFence->GetCompletedValue();
	return (ID3D12CommandAllocator*)m_D3D12AllocatorPool.RequestAllocator( CompletedValue );
#endif
	return NULL;
}

// FCommandManager
//

FORCEINLINE bool FCommandManager::IsFenceComplete( uint64 FenceValue )
{
	return GetQueue( ECommandListType( FenceValue >> 56 ) ).IsFenceCompleted( FenceValue );
}

FORCEINLINE void FCommandManager::IdleGpu()
{
	m_GraphicsQueue.WaitForIdle();
	m_ComputeQueue.WaitForIdle(); // TODO: Add Compute functionality
}

FORCEINLINE FCommandQueue& FCommandManager::GetQueue( const ECommandListType& Type )
{
	switch (Type)
	{
	case ECommandListType::CLT_Compute: 
		return m_ComputeQueue;
	case ECommandListType::CLT_Direct:
	default:
		return m_GraphicsQueue;
		R_LOG( Warning, TEXT( "Unidentified enum value provided when getting command queue: %i! Returning graphics queue as fallback." ), Type );
	}
}

FORCEINLINE FCommandQueue& FCommandManager::GetGraphicsQueue()
{
	return m_GraphicsQueue; 
}

FORCEINLINE FCommandQueue& FCommandManager::GetComputeQueue()
{ 
	return m_ComputeQueue; 
}
