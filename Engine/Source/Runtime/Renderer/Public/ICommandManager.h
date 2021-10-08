#pragma once

#include "RendererFwd.h"
#include "CommonEnums.h"

class FRenderDevice;
class FCommandContext;
class ICommandAllocator;


class RENDER_API FCommandQueue
{
	friend class FCommandManager;
public:
	virtual void* GetNativeQueue() = 0;

	virtual void WaitForFence(uint64 FenceValue) = 0;
	virtual bool IsFenceCompleted(uint64 FenceValue) = 0;
	virtual uint64 IncrementFence() = 0;
	void WaitForIdle() { WaitForFence(IncrementFence()); }

protected:
	FCommandQueue(const ECommandListType& Type)
		: m_Type(Type)
	{
	}
	virtual ~FCommandQueue()
	{
	}

	ECommandListType m_Type;
};

class RENDER_API FCommandManager
{
	friend class FRenderContextFactoryD3D12;
	friend class FCommandManagerD3D12;
	friend class FRenderContext;
public:
	virtual void Initialize(FRenderDevice* pDevice) = 0;

	virtual void CreateNewCommandContext(const ECommandListType& Type, FCommandContext** pContext, void** pData) = 0;

	virtual void WaitForFence(uint64 Value) = 0;
	inline virtual void IdleGpu()
	{
		m_pGraphicsQueue->WaitForIdle();
		//m_pComputeQueue->WaitForIdle(); // TODO: Add Compute functionality
	}

	inline FCommandQueue* GetQueue(ECommandListType Type)
	{
		switch (Type)
		{
		case ECommandListType::CLT_Direct: return m_pGraphicsQueue;
		case ECommandListType::CLT_Compute: return m_pComputeQueue;
		default:
			R_LOG(Warning, TEXT("Unidentified enum value provided when getting command queue: %i"), Type);
			return (FCommandQueue*)(NULL);
		}
	}
	inline FCommandQueue* GetGraphicsQueue()	const { return m_pGraphicsQueue; }
	inline FCommandQueue* GetComputeQueue()		const { return m_pComputeQueue; }

	// Test to see if a fence has already been reached
	inline bool IsFenceComplete(uint64 FenceValue)
	{
		return GetQueue(ECommandListType(FenceValue >> 56))->IsFenceCompleted(FenceValue);
	}

protected:
	FCommandManager()
		: m_pDeviceRef(NULL)
		, m_pGraphicsQueue(NULL)
		, m_pComputeQueue(NULL)
	{
	}
	virtual ~FCommandManager()
	{
		UnInitialize();
	}

	virtual void UnInitialize()
	{
		m_pDeviceRef = NULL;

		m_pGraphicsQueue = NULL;
		m_pComputeQueue = NULL;
	}

	inline void SetGraphicsQueue(FCommandQueue* pCommandQueue) { m_pGraphicsQueue = pCommandQueue; }

	FRenderDevice* m_pDeviceRef;

	// Implementations of this interface control the lifetime of these objects.
	//
	FCommandQueue* m_pGraphicsQueue;
	FCommandQueue* m_pComputeQueue;
};
