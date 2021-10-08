#include "RendererPCH.h"

#include "Color.h"
#include "RendererCore.h"
#include "CommandContextD3D12.h"
#include "CommandManagerD3D12.h"
#include "PipelineStateD3D12.h"
#include "ColorBufferD3D12.h"
#include "DepthBufferD3D12.h"
#include "VertexBufferD3D12.h"
#include "IndexBufferD3D12.h"
#include "ConstantBufferManagerD3D12.h"
#include "RootSignatureD3D12.h"
#include "TextureD3D12.h"
#include "TextureManagerD3D12.h"



// ----------------
//  Context Manager
// ----------------

FContextManagerD3D12::FContextManagerD3D12()
{
}

FContextManagerD3D12::~FContextManagerD3D12()
{
}

FCommandContext* FContextManagerD3D12::AllocateContext(ECommandListType Type)
{
	m_ContextAllocationMutex.Enter();

	auto& AvailableContexts = m_AvailableContexts[Type];

	FCommandContext* ret = nullptr;
	if (AvailableContexts.empty())
	{
		ret = new FCommandContextD3D12(Type);
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


	FCommandContextD3D12* pD3D12Context = DCast<FCommandContextD3D12*>(ret);
	HE_ASSERT(pD3D12Context != NULL);

	HE_ASSERT(pD3D12Context->m_Type == Type);

	m_ContextAllocationMutex.Exit();
	return ret;
}

void FContextManagerD3D12::FreeContext(FCommandContext* pContext)
{
	HE_ASSERT(pContext != NULL);
	m_ContextAllocationMutex.Enter();

	FCommandContextD3D12* pD3D12Context = DCast<FCommandContextD3D12*>(pContext);
	HE_ASSERT(pD3D12Context != NULL);

	m_AvailableContexts[pD3D12Context->m_Type].push(pContext);

	m_ContextAllocationMutex.Exit();
}

void FContextManagerD3D12::DestroyAllContexts()
{

}


// ----------------
//  Command Context
// ----------------

FCommandContextD3D12::FCommandContextD3D12(const ECommandListType& Type)
	: FCommandContext(Type)
	, m_NumBarriersToFlush(0u)
	, m_pID3D12CommandList(NULL)
	, m_pID3D12CurrentCmdAllocator(NULL)
	, m_DynamicViewDescriptorHeap(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	, m_DynamicSamplerDescriptorHeap(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
	, m_CpuLinearAllocator(LAT_CpuWritable)
	, m_GpuLinearAllocator(LAT_GpuExclusive)
{
	ZeroMemory(m_ResourceBarrierBuffer, _countof(m_ResourceBarrierBuffer) * sizeof(D3D12_RESOURCE_BARRIER));
	m_D3DCmdListType = (D3D12_COMMAND_LIST_TYPE)Type;
	ZeroMemory(m_CurrentDescriptorHeaps, sizeof(m_CurrentDescriptorHeaps));
}

FCommandContextD3D12::~FCommandContextD3D12()
{
	UnInitialize();
}

void FCommandContextD3D12::BindDescriptorHeaps()
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

void FCommandContextD3D12::Initialize()
{
	FCommandContext* pThisContext = RCast<FCommandContext*>(this);
	void** pThisAllocator = RCast<void**>(&m_pID3D12CurrentCmdAllocator);

	GCommandManager->CreateNewCommandContext(m_Type, &pThisContext, pThisAllocator);
}

void FCommandContextD3D12::UnInitialize()
{
	HE_COM_SAFE_RELEASE(m_pID3D12CommandList);
}

void FCommandContextD3D12::Reset()
{
	// We only call Reset() on previously freed contexts.  The command list persists, but we must
	// request a new allocator.
	HE_ASSERT(m_pID3D12CommandList != nullptr && m_pID3D12CurrentCmdAllocator == nullptr);

	m_pID3D12CurrentCmdAllocator = RCast<FCommandQueueD3D12*>(GCommandManager->GetQueue(m_Type))->RequestAllocator();
	m_pID3D12CommandList->Reset(m_pID3D12CurrentCmdAllocator, NULL);

	// TODO Reset root signature
	BindDescriptorHeaps();

}

void FCommandContextD3D12::BeginDebugMarker(const TChar* Message)
{
#if R_TRACK_RENDER_EVENTS
	PIXBeginEvent(m_pID3D12CommandList, 0, Message);
#endif
}

void FCommandContextD3D12::EndDebugMarker()
{
#if R_TRACK_RENDER_EVENTS
	PIXEndEvent(m_pID3D12CommandList);
#endif
}

void FCommandContextD3D12::ClearState(FPipelineState* pNewPipelineState)
{
	ID3D12PipelineState* pD3D12PipeState = NULL;
	if (pNewPipelineState != NULL)
	{
		pD3D12PipeState = RCast<ID3D12PipelineState*>(DCast<FPipelineStateD3D12*>(pNewPipelineState)->GetNativePSO());
	}
	m_pID3D12CommandList->ClearState(pD3D12PipeState);
}

void FCommandContextD3D12::OMSetRenderTargets(uint32 NumRTVs, const FColorBuffer* Targets[], FDepthBuffer* pDepthBuffer)
{
	constexpr uint32 cx_MaxRTVBinds = 12;
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandles[cx_MaxRTVBinds];
	ZeroMemory(&RTVHandles, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * cx_MaxRTVBinds);

	for (uint32 i = 0; i < NumRTVs; ++i)
	{
		const FColorBufferD3D12* pBuffer = DCast<const FColorBufferD3D12*>(Targets[i]);
		HE_ASSERT(pBuffer != NULL);

		RTVHandles[i] = pBuffer->GetRTVHandle();
	}

	if (pDepthBuffer != NULL)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle;
		FDepthBufferD3D12* pD3D12DepthBuffer = DCast<FDepthBufferD3D12*>(pDepthBuffer);
		DSVHandle = pD3D12DepthBuffer->GetDSV();
		m_pID3D12CommandList->OMSetRenderTargets(NumRTVs, RTVHandles, false, &DSVHandle);
	}
	else
	{
		m_pID3D12CommandList->OMSetRenderTargets(NumRTVs, RTVHandles, false, NULL);
	}
}

void FCommandContextD3D12::ClearDepth(FDepthBuffer& DepthBuffer)
{
	FDepthBufferD3D12* pD3D12DepthBuffer = DCast<FDepthBufferD3D12*>(&DepthBuffer);
	D3D12_CPU_DESCRIPTOR_HANDLE  DSVHandle = pD3D12DepthBuffer->GetDSV();

	m_pID3D12CommandList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH, DepthBuffer.GetClearDepth(), DepthBuffer.GetClearStencil(), 0, NULL);
}

void FCommandContextD3D12::RSSetViewPorts(uint32 NumViewPorts, const FViewPort* ViewPorts)
{
	m_pID3D12CommandList->RSSetViewports(NumViewPorts, RCast<const D3D12_VIEWPORT*>(ViewPorts));
}

void FCommandContextD3D12::RSSetScissorRects(uint32 NumScissorRects, const FRect* ScissorRects)
{
	m_pID3D12CommandList->RSSetScissorRects(NumScissorRects, RCast<const RECT*>(ScissorRects));
}

void FCommandContextD3D12::SetPrimitiveTopologyType(EPrimitiveTopology TopologyType)
{
	m_pID3D12CommandList->IASetPrimitiveTopology((D3D12_PRIMITIVE_TOPOLOGY)TopologyType);
}

void FCommandContextD3D12::ClearColorBuffer(FColorBuffer& Buffer, const FRect& FRect)
{
	FlushResourceBarriers();
	FColorBufferD3D12* pD3D12ColorBuffer = DCast<FColorBufferD3D12*>(&Buffer);
	HE_ASSERT(pD3D12ColorBuffer != NULL);

	FColor ClearColor = pD3D12ColorBuffer->GetClearColor();
	m_pID3D12CommandList->ClearRenderTargetView(pD3D12ColorBuffer->GetRTVHandle(), &ClearColor.R, 1, RCast<const D3D12_RECT*>(&FRect));
}

void FCommandContextD3D12::CreateTexture2D()
{
}

void FCommandContextD3D12::CreateBuffer()
{
}

void FCommandContextD3D12::SetDescriptorHeap(EResourceHeapType Type, FDescriptorHeap* HeapPtr)
{
	FDynamicDescriptorHeapD3D12* pID3D12Heap = RCast<FDynamicDescriptorHeapD3D12*>(HeapPtr);
	HE_ASSERT(pID3D12Heap != NULL);

	if (m_CurrentDescriptorHeaps[Type] != pID3D12Heap->GetNativeHeap())
	{
		m_CurrentDescriptorHeaps[Type] = RCast<ID3D12DescriptorHeap*>(pID3D12Heap->GetNativeHeap());
		BindDescriptorHeaps();
	}
}

DynAlloc FCommandContextD3D12::ReserveUploadMemory(uint64 SizeInBytes)
{
	return m_CpuLinearAllocator.Allocate(SizeInBytes);
}

void FCommandContextD3D12::UpdateSubresources(FGpuResource& Destination, FGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, FSubResourceData& SubresourceData)
{
	FGpuResourceD3D12* pD3D12DestGpuResource = DCast<FGpuResourceD3D12*>(&Destination);
	FGpuResourceD3D12* pD3D12IntGpuResource = DCast<FGpuResourceD3D12*>(&Intermediate);

	ID3D12Resource* pID3D12Destination = pD3D12DestGpuResource->GetResource();
	ID3D12Resource* pID3D12Intermediate = pD3D12IntGpuResource->GetResource();
	D3D12_SUBRESOURCE_DATA SRData = {};
	SRData.pData = SubresourceData.pData;
	SRData.RowPitch = SubresourceData.RowPitch;
	SRData.SlicePitch = SubresourceData.SlicePitch;

	::UpdateSubresources(m_pID3D12CommandList, pID3D12Destination, pID3D12Intermediate, IntermediateOffset, FirstSubresource, NumSubresources, &SRData);
}

void FCommandContextD3D12::SetDepthBufferAsTexture(uint32 RootParameterIndex, const FDepthBuffer* pDepthBuffer)
{
	const FDepthBufferD3D12* pD3D12DepthBuffer = DCast<const FDepthBufferD3D12*>(pDepthBuffer);
	m_DynamicViewDescriptorHeap.SetGraphicsDescriptorHandles(RootParameterIndex, 0, 1, &pD3D12DepthBuffer->GetDepthSRV());
}

void FCommandContextD3D12::SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const FColorBuffer* Buffers[])
{
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> Handles;
	for (uint32 i = 0; i < Count; ++i)
	{
		const FColorBufferD3D12* pD3D12ColorBuffer = DCast<const FColorBufferD3D12*>(Buffers[i]);
		HE_ASSERT(pD3D12ColorBuffer != NULL);
		Handles.push_back(pD3D12ColorBuffer->GetSRVHandle());
	}
	m_DynamicViewDescriptorHeap.SetGraphicsDescriptorHandles(RootParameterIndex, Offset, Count, Handles.data());
}

void FCommandContextD3D12::SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, FColorBuffer* Buffer)
{
	const FColorBuffer* Buffers[] = { Buffer };
	SetColorBuffersAsTextures(RootParameterIndex, Offset, 1, Buffers);
}

void FCommandContextD3D12::BindVertexBuffer(uint32 Slot, FVertexBuffer& VertexBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW* pView = RCast<D3D12_VERTEX_BUFFER_VIEW*>(VertexBuffer.GetNativeBufferView());
	m_pID3D12CommandList->IASetVertexBuffers(Slot, 1, pView);
}

void FCommandContextD3D12::BindIndexBuffer(FIndexBuffer& IndexBuffer)
{
	D3D12_INDEX_BUFFER_VIEW* pView = RCast<D3D12_INDEX_BUFFER_VIEW*>(IndexBuffer.GetNativeBufferView());
	m_pID3D12CommandList->IASetIndexBuffer(pView);
}

void FCommandContextD3D12::SetGraphicsConstantBuffer(uint32 RootParameterIndex, FConstantBuffer* pConstantBuffer)
{
	FConstantBufferD3D12& D3D12Cb = *DCast<FConstantBufferD3D12*>(pConstantBuffer);
	D3D12Cb.UploadBuffer();
	D3D12_GPU_VIRTUAL_ADDRESS Address = D3D12Cb.GetGPUVirtualAddress();
	m_pID3D12CommandList->SetGraphicsRootConstantBufferView(RootParameterIndex, Address);
}

void FCommandContextD3D12::SetTexture(uint32 RootParameterIndex, HTextureRef& pTexture)
{
	const HTextureD3D12* pD3D12Tex = DCast<const HTextureD3D12*>(pTexture.Get());
	D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle;

	if (pD3D12Tex == NULL || !pTexture.IsValid())
	{
		// If the texture is invalid or incomplete: bind a default texture instead.
		GpuHandle.ptr = DCast<HTextureD3D12*>(GDefaultTextures[DT_BlackOpaque2D])->GetShaderVisibleDescriptorHandle().GetGpuPtr();
	}
	else
	{
		GpuHandle.ptr = pD3D12Tex->GetShaderVisibleDescriptorHandle().GetGpuPtr();
	}

	m_pID3D12CommandList->SetGraphicsRootDescriptorTable(RootParameterIndex, GpuHandle);
}

void FCommandContextD3D12::SetPipelineState(FPipelineState& Pipeline)
{
	ID3D12PipelineState* pD3D12Pipeline = RCast<ID3D12PipelineState*>(Pipeline.GetNativePSO());
	HE_ASSERT(pD3D12Pipeline != NULL);

	m_pID3D12CommandList->SetPipelineState(pD3D12Pipeline);
}

void FCommandContextD3D12::SetGraphicsRootSignature(FRootSignature& Signature)
{
	ID3D12RootSignature* pD3D12Signature = RCast<ID3D12RootSignature*>(Signature.GetNativeSignature());
	HE_ASSERT(pD3D12Signature != NULL);

	m_pID3D12CommandList->SetGraphicsRootSignature(pD3D12Signature);

	m_DynamicViewDescriptorHeap.ParseGraphicsRootSignature(Signature);
	m_DynamicSamplerDescriptorHeap.ParseGraphicsRootSignature(Signature);
}

void FCommandContextD3D12::Draw(uint32 VertexCount, uint32 VertexStartOffset)
{
	DrawInstanced(VertexCount, 1, VertexStartOffset, 0);
}

void FCommandContextD3D12::DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation, int32 BaseVertexLocation)
{
	DrawIndexedInstanced(IndexCount, 1, StartIndexLocation, BaseVertexLocation, 0);
}

void FCommandContextD3D12::DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation, uint32 StartInstanceLocation)
{
	FlushResourceBarriers();
	m_DynamicViewDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_DynamicSamplerDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_pID3D12CommandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void FCommandContextD3D12::DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation)
{
	FlushResourceBarriers();
	m_DynamicViewDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_DynamicSamplerDescriptorHeap.CommitGraphicsRootDescriptorTables(m_pID3D12CommandList);
	m_pID3D12CommandList->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

void FCommandContextD3D12::TransitionResource(FGpuResource& Resource, EResourceState NewState, bool FlushImmediate/* = false*/)
{
	EResourceState OldState = Resource.GetUsageState();

	if (NewState != OldState)
	{
		HE_ASSERT(m_NumBarriersToFlush < 16); // Exceeded arbitrary limit on buffered barriers.
		D3D12_RESOURCE_BARRIER& BarrierDesc = m_ResourceBarrierBuffer[m_NumBarriersToFlush++];


		BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		BarrierDesc.Transition.pResource = DCast<FGpuResourceD3D12*>(&Resource)->GetResource();
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


uint64 FCommandContextD3D12::Flush(bool WaitForCompletion/* = false*/)
{
	FlushResourceBarriers();

	HE_ASSERT(m_pID3D12CurrentCmdAllocator != NULL);

	FCommandQueueD3D12* pQueue = DCast<FCommandQueueD3D12*>(GCommandManager->GetGraphicsQueue());
	uint64 FenceValue = pQueue->ExecuteCommandList(m_pID3D12CommandList);

	if (WaitForCompletion)
		GCommandManager->WaitForFence(FenceValue);

	m_pID3D12CommandList->Reset(m_pID3D12CurrentCmdAllocator, NULL);

	// TODO: Set root signature

	BindDescriptorHeaps();

	return 0;

}

uint64 FCommandContextD3D12::End(bool WaitForCompletion/* = false*/)
{
	FlushResourceBarriers();
	EndDebugMarker();

	FCommandQueueD3D12* pQueue = DCast<FCommandQueueD3D12*>(GCommandManager->GetQueue(m_Type));

	uint64 FenceValue = pQueue->ExecuteCommandList(m_pID3D12CommandList);
	pQueue->DiscardAllocator(FenceValue, m_pID3D12CurrentCmdAllocator);
	m_pID3D12CurrentCmdAllocator = NULL;

	m_CpuLinearAllocator.CleanupUsedPages(FenceValue);
	m_GpuLinearAllocator.CleanupUsedPages(FenceValue);
	m_DynamicViewDescriptorHeap.CleanupUsedHeaps(FenceValue);
	m_DynamicSamplerDescriptorHeap.CleanupUsedHeaps(FenceValue);

	if (WaitForCompletion)
		pQueue->WaitForFence(FenceValue);

	GContextManager->FreeContext(this);

	return 0;
}

void FCommandContextD3D12::FlushResourceBarriers()
{
	if (m_NumBarriersToFlush > 0)
	{
		m_pID3D12CommandList->ResourceBarrier(m_NumBarriersToFlush, m_ResourceBarrierBuffer);
		m_NumBarriersToFlush = 0u;
	}
}
