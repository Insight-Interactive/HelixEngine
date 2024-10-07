// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CriticalSection.h"
#include "RendererCore.h"
#include "LinearAllocator.h"
#include "DynamicDescriptorHeap.h"
#include "TextureManager.h"


static const int kContextPoolSize = 4;
static const int kAvailableContextPoolSize = kContextPoolSize;

class FCommandContext;
class FColor;

/*
	Manages command context lifetime and creation.
*/
class FContextManager
{
	friend class FRenderContext;
public:
	FContextManager();
	~FContextManager();

	FCommandContext* AllocateContext(ECommandListType Type);
	void FreeContext(FCommandContext* pContext);
	void DestroyAllContexts();

protected:
	std::vector<FCommandContext*> m_ContextPool[kContextPoolSize];
	std::queue<FCommandContext*> m_AvailableContexts[kAvailableContextPoolSize];
	CriticalSection m_ContextAllocationMutex;
	
};

/*
	Command reciever that executes graphics commands. Draw, texture binds, etc.
*/
class FCommandContext
{
	friend FContextManager;
public:
	void* GetNativeContext();
	void** GetNativeContextAddress();

	void Initialize();
	void UnInitialize();
	void Reset();

	static FCommandContext& Begin(const TChar* ID);
	uint64 End(bool WaitForCompletion = false);
	uint64 Flush(bool WaitForCompletion = false);

	void BeginDebugMarker(const WChar* Message);
	void EndDebugMarker();
	void ClearState(FPipelineState* pNewPipelineState);

	void OMSetRenderTargets(uint32 NumRTVs, const FColorBuffer* Targets[], FDepthBuffer* pDepthBuffer);

	void RSSetViewPorts(uint32 NumViewPorts, const FViewPort* ViewPorts);
	void RSSetScissorRects(uint32 NumScissorRects, const FRect* ScissorRects);

	void ClearColorBuffer(FColorBuffer& Buffer, const FRect& FRect);
	void ClearDepth(FDepthBuffer& DepthBuffer);

	void CreateTexture2D();
	void CreateBuffer();

	void SetDescriptorHeap(EResourceHeapType Type, void* NativeHeapPtr);
	DynAlloc ReserveUploadMemory( const uint64& SizeInBytes );

	void UpdateSubresources(FGpuResource& Destination, FGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, FSubResourceData& SubresourceData);

	void SetDepthBufferAsTexture(uint32 RootParameterIndex, const FDepthBuffer* pDepthBuffer);
	void SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const FColorBuffer* Buffers[]);
	void SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, FColorBuffer* Buffer);
	void BindVertexBuffer(uint32 Slot, FVertexBuffer& Vertexbuffer);
	void BindIndexBuffer(FIndexBuffer& IndexBuffer);
	void SetGraphicsConstantBuffer(uint32 RootParameterIndex, FConstantBufferInterface& ConstantBuffer, bool Upload = true);
	void SetTexture(uint32 Slot, HTexture& Texture);

	void SetPipelineState(FPipelineState& Pipeline);
	void SetGraphicsRootSignature(FRootSignature& Signature);
	void SetPrimitiveTopologyType(EPrimitiveTopology TopologyType);

	void Draw(uint32 VertexCount, uint32 VertexStartOffset);
	void DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation, int32 BaseVertexLocation);
	void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation, uint32 StartInstanceLocation);
	void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation);

	void TransitionResource(FGpuResource& Resource, EResourceState NewState, bool FlushImmediate = false);

	void FlushResourceBarriers();

	void SetID(const TChar* NewID) { m_ID = NewID; }

protected:
	FCommandContext( const ECommandListType& Type );
	~FCommandContext();

	void BindDescriptorHeaps();

	const TChar* m_ID;
	ECommandListType m_Type;

#if R_WITH_D3D12
	D3D12_COMMAND_LIST_TYPE m_D3DCmdListType;
	uint32 m_NumBarriersToFlush;
	D3D12_RESOURCE_BARRIER m_ResourceBarrierBuffer[16];

	ID3D12GraphicsCommandList* m_pID3D12CommandList;
	ID3D12CommandAllocator* m_pID3D12CurrentCmdAllocator;

	ID3D12DescriptorHeap* m_CurrentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	FDynamicDescriptorHeap m_DynamicViewDescriptorHeap;		// HEAP_TYPE_CBV_SRV_UAV
	FDynamicDescriptorHeap m_DynamicSamplerDescriptorHeap;	// HEAP_TYPE_SAMPLER

	FLinearAllocator m_CpuLinearAllocator;
	FLinearAllocator m_GpuLinearAllocator;

#endif

};


//
// Inline function implementations
//

FORCEINLINE void* FCommandContext::GetNativeContext()
{
#if R_WITH_D3D12
	return (void*)m_pID3D12CommandList;
#endif
}

FORCEINLINE void** FCommandContext::GetNativeContextAddress()
{
#if R_WITH_D3D12
	return (void**)&m_pID3D12CommandList;
#endif
}
