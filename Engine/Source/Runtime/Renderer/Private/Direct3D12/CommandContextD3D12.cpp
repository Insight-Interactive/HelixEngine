#include "RendererPCH.h"
#if R_WITH_D3D12

#include "CommandContext.h"

#include "Color.h"
#include "RendererCore.h"
#include "CommandManager.h"
#include "PipelineState.h"
#include "ColorBuffer.h"
#include "DepthBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "Texture.h"
#include "TextureManager.h"


// ----------------
//  Context Manager
// ----------------

FContextManager::FContextManager()
{
}

FContextManager::~FContextManager()
{
	for (size_t i = 0; i < kContextPoolSize; ++i)
	{
		for (size_t j = 0; j < m_ContextPool[i].size(); ++j)
		{
			delete m_ContextPool[i][j];
		}
	}
}

FCommandContext* FContextManager::AllocateContext(ECommandListType Type)
{
	m_ContextAllocationMutex.Enter();

	auto& AvailableContexts = m_AvailableContexts[Type];

	FCommandContext* ret = nullptr;
	if (AvailableContexts.empty())
	{
		ret = new FCommandContext(Type);
		m_ContextPool[Type].emplace_back(ret);
		ret->Initialize();
	}
	else
	{
		ret = AvailableContexts.front();
		AvailableContexts.pop();
		ret->Reset();
	}
	HE_ASSERT(ret != nullptr);


	FCommandContext* pD3D12Context = DCast<FCommandContext*>(ret);
	HE_ASSERT(pD3D12Context != NULL);

	HE_ASSERT(pD3D12Context->m_Type == Type);

	m_ContextAllocationMutex.Exit();
	return ret;
}

void FContextManager::FreeContext(FCommandContext* pContext)
{
	HE_ASSERT(pContext != NULL);
	m_ContextAllocationMutex.Enter();

	FCommandContext* pD3D12Context = DCast<FCommandContext*>(pContext);
	HE_ASSERT(pD3D12Context != NULL);

	m_AvailableContexts[pD3D12Context->m_Type].push(pContext);

	m_ContextAllocationMutex.Exit();
}

void FContextManager::DestroyAllContexts()
{

}


// ----------------
//  Command Context
// ----------------

FCommandContext::FCommandContext(const ECommandListType& Type)
	: m_NumBarriersToFlush(0u)
	, m_Type(Type)
	, m_pID3D12CommandList(NULL)
	, m_pID3D12CurrentCmdAllocator(NULL)
	, m_DynamicViewDescriptorHeap(*this, RHT_CBV_SRV_UAV )
	, m_DynamicSamplerDescriptorHeap(*this, RHT_Sampler )
	, m_CpuLinearAllocator(LAT_CpuWritable)
	, m_GpuLinearAllocator(LAT_GpuExclusive)
{
	ZeroMemory(m_ResourceBarrierBuffer, _countof(m_ResourceBarrierBuffer) * sizeof(D3D12_RESOURCE_BARRIER));
	m_D3DCmdListType = (D3D12_COMMAND_LIST_TYPE)Type;
	ZeroMemory(m_CurrentDescriptorHeaps, sizeof(m_CurrentDescriptorHeaps));
}

FCommandContext::~FCommandContext()
{
	UnInitialize();
}

void FCommandContext::BindDescriptorHeaps()
{
	UINT NonNullHeaps = 0;
	ID3D12DescriptorHeap* HeapsToBind[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
	for (UINT i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
	{
		ID3D12DescriptorHeap* HeapIter = m_CurrentDescriptorHeaps[i];
		if (HeapIter != nullptr)
			HeapsToBind[NonNullHeaps++] = HeapIter;
	}

	if (NonNullHeaps > 0)
		m_pID3D12CommandList->SetDescriptorHeaps(NonNullHeaps, HeapsToBind);
}

void FCommandContext::Initialize()
{
	FCommandContext* pThisContext = RCast<FCommandContext*>(this);
	void** pThisAllocator = RCast<void**>(&m_pID3D12CurrentCmdAllocator);

	GCommandManager.CreateNewCommandContext(m_Type, &pThisContext, pThisAllocator);
}

void FCommandContext::UnInitialize()
{
	HE_COM_SAFE_RELEASE(m_pID3D12CommandList);
}

void FCommandContext::Reset()
{
	// We only call Reset() on previously freed contexts.  The command list persists, but we must
	// request a new allocator.
	HE_ASSERT(m_pID3D12CommandList != nullptr && m_pID3D12CurrentCmdAllocator == nullptr);

	m_pID3D12CurrentCmdAllocator = (ID3D12CommandAllocator*)GCommandManager.GetQueue(m_Type).RequestAllocator();
	m_pID3D12CommandList->Reset(m_pID3D12CurrentCmdAllocator, NULL);

	// TODO Reset root signature
	BindDescriptorHeaps();
}

void FCommandContext::BeginDebugMarker(const WChar* Message)
{
#if R_TRACK_RENDER_EVENTS
	PIXBeginEvent(m_pID3D12CommandList, 0, Message);
#endif
}

void FCommandContext::EndDebugMarker()
{
#if R_TRACK_RENDER_EVENTS
	PIXEndEvent(m_pID3D12CommandList);
#endif
}

void FCommandContext::ClearState(FPipelineState* pNewPipelineState)
{
	ID3D12PipelineState* pD3D12PipeState = NULL;
	if (pNewPipelineState != NULL)
	{
		pD3D12PipeState = (ID3D12PipelineState*)pNewPipelineState->GetNativePSO();
	}
	m_pID3D12CommandList->ClearState(pD3D12PipeState);
}

void FCommandContext::OMSetRenderTargets(uint32 NumRTVs, const FColorBuffer* Targets[], FDepthBuffer* pDepthBuffer)
{
	constexpr uint32 cx_MaxRTVBinds = 12;
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandles[cx_MaxRTVBinds];
	ZeroMemory(&RTVHandles, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * cx_MaxRTVBinds);

	for (uint32 i = 0; i < NumRTVs; ++i)
	{
		RTVHandles[i] = Targets[i]->GetRTVHandle();
	}

	if (pDepthBuffer != NULL)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle;
		DSVHandle = pDepthBuffer->GetDSV();
		m_pID3D12CommandList->OMSetRenderTargets(NumRTVs, RTVHandles, false, &DSVHandle);
	}
	else
	{
		m_pID3D12CommandList->OMSetRenderTargets(NumRTVs, RTVHandles, false, NULL);
	}
}

void FCommandContext::ClearDepth(FDepthBuffer& DepthBuffer)
{
	D3D12_CPU_DESCRIPTOR_HANDLE  DSVHandle = DepthBuffer.GetDSV();

	m_pID3D12CommandList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH, DepthBuffer.GetClearDepth(), DepthBuffer.GetClearStencil(), 0, NULL);
}

void FCommandContext::RSSetViewPorts(uint32 NumViewPorts, const FViewPort* ViewPorts)
{
	m_pID3D12CommandList->RSSetViewports(NumViewPorts, RCast<const D3D12_VIEWPORT*>(ViewPorts));
}

void FCommandContext::RSSetScissorRects(uint32 NumScissorRects, const FRect* ScissorRects)
{
	m_pID3D12CommandList->RSSetScissorRects(NumScissorRects, RCast<const RECT*>(ScissorRects));
}

void FCommandContext::SetPrimitiveTopologyType(EPrimitiveTopology TopologyType)
{
	m_pID3D12CommandList->IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)TopologyType);
}

void FCommandContext::ClearColorBuffer(FColorBuffer& Buffer, const FRect& FRect)
{
	FlushResourceBarriers();
	FColor ClearColor = Buffer.GetClearColor();
	m_pID3D12CommandList->ClearRenderTargetView( Buffer.GetRTVHandle(), &ClearColor.R, 1, RCast<const D3D12_RECT*>(&FRect));
}

void FCommandContext::CreateTexture2D()
{
}

void FCommandContext::CreateBuffer()
{
}

void FCommandContext::SetDescriptorHeap(EResourceHeapType Type, void* NativeHeapPtr )
{
	ID3D12DescriptorHeap* pID3D12Heap = (ID3D12DescriptorHeap*)NativeHeapPtr;
	if (m_CurrentDescriptorHeaps[Type] != pID3D12Heap)
	{
		m_CurrentDescriptorHeaps[Type] = pID3D12Heap;
		BindDescriptorHeaps();
	}
}

DynAlloc FCommandContext::ReserveUploadMemory(const uint64& SizeInBytes)
{
	return m_CpuLinearAllocator.Allocate(SizeInBytes);
}

void FCommandContext::UpdateSubresources(FGpuResource& Destination, FGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, FSubResourceData& SubresourceData)
{
	ID3D12Resource* pID3D12Destination = (ID3D12Resource*)Destination.GetResource();
	ID3D12Resource* pID3D12Intermediate = (ID3D12Resource*)Intermediate.GetResource();
	D3D12_SUBRESOURCE_DATA SRData = {};
	SRData.pData = SubresourceData.pData;
	SRData.RowPitch = SubresourceData.RowPitch;
	SRData.SlicePitch = SubresourceData.SlicePitch;

	::UpdateSubresources(m_pID3D12CommandList, pID3D12Destination, pID3D12Intermediate, IntermediateOffset, FirstSubresource, NumSubresources, &SRData);
}

void FCommandContext::SetDepthBufferAsTexture(uint32 RootParameterIndex, const FDepthBuffer* pDepthBuffer)
{
	m_DynamicViewDescriptorHeap.SetGraphicsDescriptorHandles(RootParameterIndex, 0, 1, &pDepthBuffer->GetDepthSRV());
}

void FCommandContext::SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const FColorBuffer* Buffers[])
{
	D3D12_CPU_DESCRIPTOR_HANDLE* Handles = (D3D12_CPU_DESCRIPTOR_HANDLE*)HE_StackAlloc( sizeof( D3D12_CPU_DESCRIPTOR_HANDLE ) * Count );
	HE_ASSERT( Handles != NULL ); // Failed to allocate stack memory for descriptor handles.
	for (uint32 i = 0; i < Count; ++i)
	{
		Handles++;
		*Handles = Buffers[i]->GetSRVHandle();
	}
	m_DynamicViewDescriptorHeap.SetGraphicsDescriptorHandles(RootParameterIndex, Offset, Count, Handles);
}

void FCommandContext::SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, FColorBuffer* Buffer)
{
	const FColorBuffer* Buffers[] = { Buffer };
	SetColorBuffersAsTextures(RootParameterIndex, Offset, 1, Buffers);
}

void FCommandContext::BindVertexBuffer(uint32 Slot, FVertexBuffer& VertexBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW* pView = RCast<D3D12_VERTEX_BUFFER_VIEW*>(VertexBuffer.GetNativeBufferView());
	m_pID3D12CommandList->IASetVertexBuffers(Slot, 1, pView);
}

void FCommandContext::BindIndexBuffer(FIndexBuffer& IndexBuffer)
{
	D3D12_INDEX_BUFFER_VIEW* pView = RCast<D3D12_INDEX_BUFFER_VIEW*>(IndexBuffer.GetNativeBufferView());
	m_pID3D12CommandList->IASetIndexBuffer(pView);
}

void FCommandContext::SetGraphicsConstantBuffer(uint32 RootParameterIndex, FConstantBufferInterface& ConstantBuffer)
{
	ConstantBuffer.UploadBuffer();
	D3D12_GPU_VIRTUAL_ADDRESS Address = ConstantBuffer.GetGPUVirtualAddress();
	m_pID3D12CommandList->SetGraphicsRootConstantBufferView(RootParameterIndex, Address);
}

void FCommandContext::SetTexture(uint32 RootParameterIndex, HTextureRef& pTexture)
{
	const HTexture* pD3D12Tex = pTexture.Get();
	D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle;

	if (pD3D12Tex == NULL || !pTexture.IsValid())
	{
		// If the texture is invalid or incomplete: bind a default texture instead.
		GpuHandle.ptr = GDefaultTextures[DT_BlackOpaque2D].GetShaderVisibleDescriptorHandle().GetGpuPtr();
	}
	else
	{
		GpuHandle.ptr = pD3D12Tex->GetShaderVisibleDescriptorHandle().GetGpuPtr();
	}

	m_pID3D12CommandList->SetGraphicsRootDescriptorTable(RootParameterIndex, GpuHandle);
}

void FCommandContext::SetPipelineState(FPipelineState& Pipeline)
{
	ID3D12PipelineState* pD3D12Pipeline = RCast<ID3D12PipelineState*>(Pipeline.GetNativePSO());
	HE_ASSERT(pD3D12Pipeline != NULL);

	m_pID3D12CommandList->SetPipelineState(pD3D12Pipeline);
}

void FCommandContext::SetGraphicsRootSignature(FRootSignature& Signature)
{
	ID3D12RootSignature* pD3D12Signature = RCast<ID3D12RootSignature*>(Signature.GetNativeSignature());
	HE_ASSERT(pD3D12Signature != NULL);

	m_pID3D12CommandList->SetGraphicsRootSignature(pD3D12Signature);

	m_DynamicViewDescriptorHeap.ParseGraphicsRootSignature(Signature);
	m_DynamicSamplerDescriptorHeap.ParseGraphicsRootSignature(Signature);
}

void FCommandContext::Draw(uint32 VertexCount, uint32 VertexStartOffset)
{
	DrawInstanced(VertexCount, 1, VertexStartOffset, 0);
}

void FCommandContext::DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation, int32 BaseVertexLocation)
{
	DrawIndexedInstanced(IndexCount, 1, StartIndexLocation, BaseVertexLocation, 0);
}

void FCommandContext::DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation, uint32 StartInstanceLocation)
{
	FlushResourceBarriers();
	m_DynamicViewDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_DynamicSamplerDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_pID3D12CommandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void FCommandContext::DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation)
{
	FlushResourceBarriers();
	m_DynamicViewDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_DynamicSamplerDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_pID3D12CommandList->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

void FCommandContext::TransitionResource(FGpuResource& Resource, EResourceState NewState, bool FlushImmediate/* = false*/)
{
	EResourceState OldState = Resource.GetUsageState();

	if (NewState != OldState)
	{
		HE_ASSERT(m_NumBarriersToFlush < 16); // Exceeded arbitrary limit on buffered barriers.
		D3D12_RESOURCE_BARRIER& BarrierDesc = m_ResourceBarrierBuffer[m_NumBarriersToFlush++];


		BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		BarrierDesc.Transition.pResource = (ID3D12Resource*)Resource.GetResource();
		BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		BarrierDesc.Transition.StateBefore = (D3D12_RESOURCE_STATES)OldState;
		BarrierDesc.Transition.StateAfter = (D3D12_RESOURCE_STATES)NewState;

		// Check to see if we already started the transition
		if (NewState == Resource.GetTransitionState())
		{
			BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
			Resource.SetTransitionState((EResourceState)-1);
		}
		else
			BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		Resource.SetUsageState(NewState);
	}
	//else if (NewState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	//	InsertUAVBarrier(Resource, FlushImmediate);

	if (FlushImmediate || m_NumBarriersToFlush == 16)
		FlushResourceBarriers();
}


uint64 FCommandContext::Flush(bool WaitForCompletion/* = false*/)
{
	FlushResourceBarriers();

	HE_ASSERT(m_pID3D12CurrentCmdAllocator != NULL);

	FCommandQueue& Queue = GCommandManager.GetGraphicsQueue();
	uint64 FenceValue = Queue.ExecuteCommandList(m_pID3D12CommandList);

	if (WaitForCompletion)
		GCommandManager.WaitForFence(FenceValue);

	m_pID3D12CommandList->Reset(m_pID3D12CurrentCmdAllocator, NULL);

	// TODO: Set root signature

	BindDescriptorHeaps();

	return 0;

}

uint64 FCommandContext::End(bool WaitForCompletion/* = false*/)
{
	FlushResourceBarriers();
	EndDebugMarker();

	FCommandQueue& Queue = GCommandManager.GetQueue(m_Type);

	uint64 FenceValue = Queue.ExecuteCommandList(m_pID3D12CommandList);
	Queue.DiscardAllocator(FenceValue, m_pID3D12CurrentCmdAllocator);
	m_pID3D12CurrentCmdAllocator = NULL;

	m_CpuLinearAllocator.CleanupUsedPages(FenceValue);
	m_GpuLinearAllocator.CleanupUsedPages(FenceValue);
	m_DynamicViewDescriptorHeap.CleanupUsedHeaps(FenceValue);
	m_DynamicSamplerDescriptorHeap.CleanupUsedHeaps(FenceValue);

	if (WaitForCompletion)
		Queue.WaitForFence(FenceValue);

	GContextManager.FreeContext(this);

	return 0;
}

void FCommandContext::FlushResourceBarriers()
{
	if (m_NumBarriersToFlush > 0)
	{
		m_pID3D12CommandList->ResourceBarrier(m_NumBarriersToFlush, m_ResourceBarrierBuffer);
		m_NumBarriersToFlush = 0u;
	}
}

#endif // R_WITH_D3D12
