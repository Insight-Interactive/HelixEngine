#include "RendererPCH.h"

#include "DynamicDescriptorHeapD3D12.h"

#include "RendererFwd.h"
#include "IRenderDevice.h"
#include "CommandManagerD3D12.h"
#include "CommandContextD3D12.h"
#include "RootSignatureD3D12.h"


//
// FDynamicDescriptorHeapD3D12 Implementation
//

CriticalSection FDynamicDescriptorHeapD3D12::sm_Mutex;
std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> FDynamicDescriptorHeapD3D12::sm_DescriptorHeapPool[2];
std::queue<std::pair<uint64, ID3D12DescriptorHeap*>> FDynamicDescriptorHeapD3D12::sm_RetiredDescriptorHeaps[2];
std::queue<ID3D12DescriptorHeap*> FDynamicDescriptorHeapD3D12::sm_AvailableDescriptorHeaps[2];

ID3D12DescriptorHeap* FDynamicDescriptorHeapD3D12::RequestDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType)
{
    sm_Mutex.Enter();

    uint32 idx = HeapType == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ? 1 : 0;
    FCommandManagerD3D12* CmdManager = DCast<FCommandManagerD3D12*>(GCommandManager);

    while (!sm_RetiredDescriptorHeaps[idx].empty() && CmdManager->IsFenceComplete(sm_RetiredDescriptorHeaps[idx].front().first))
    {
        sm_AvailableDescriptorHeaps[idx].push(sm_RetiredDescriptorHeaps[idx].front().second);
        sm_RetiredDescriptorHeaps[idx].pop();
    }

    if (!sm_AvailableDescriptorHeaps[idx].empty())
    {
        ID3D12DescriptorHeap* HeapPtr = sm_AvailableDescriptorHeaps[idx].front();
        sm_AvailableDescriptorHeaps[idx].pop();
        return HeapPtr;
    }
    else
    {
        ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
        HE_ASSERT(pID3D12Device != NULL);

        D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
        HeapDesc.Type = HeapType;
        HeapDesc.NumDescriptors = kNumDescriptorsPerHeap;
        HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        HeapDesc.NodeMask = 1;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> HeapPtr;
        pID3D12Device->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&HeapPtr));
        sm_DescriptorHeapPool[idx].emplace_back(HeapPtr);
        return HeapPtr.Get();
    }
    sm_Mutex.Exit();
}

void FDynamicDescriptorHeapD3D12::DiscardDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE HeapType, uint64 FenceValue, const std::vector<ID3D12DescriptorHeap*>& UsedHeaps)
{
    uint32 idx = HeapType == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ? 1 : 0;
    sm_Mutex.Enter();
    for (auto iter = UsedHeaps.begin(); iter != UsedHeaps.end(); ++iter)
        sm_RetiredDescriptorHeaps[idx].push(std::make_pair(FenceValue, *iter));
    sm_Mutex.Exit();
}

void FDynamicDescriptorHeapD3D12::RetireCurrentHeap(void)
{
    // Don't retire unused heaps.
    if (m_CurrentOffset == 0)
    {
        HE_ASSERT(m_CurrentHeapPtr == nullptr);
        return;
    }

    HE_ASSERT(m_CurrentHeapPtr != nullptr);
    m_RetiredHeaps.push_back(m_CurrentHeapPtr);
    m_CurrentHeapPtr = nullptr;
    m_CurrentOffset = 0;
}

void FDynamicDescriptorHeapD3D12::RetireUsedHeaps(uint64 fenceValue)
{
    DiscardDescriptorHeaps(m_DescriptorType, fenceValue, m_RetiredHeaps);
    m_RetiredHeaps.clear();
}

FDynamicDescriptorHeapD3D12::FDynamicDescriptorHeapD3D12(FCommandContextD3D12& OwningContext, D3D12_DESCRIPTOR_HEAP_TYPE HeapType)
    : m_OwningContext(OwningContext)
    , m_DescriptorType(HeapType)
{
    m_CurrentHeapPtr = nullptr;
    m_CurrentOffset = 0;

    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
    HE_ASSERT(pID3D12Device != NULL);

    m_DescriptorSize = pID3D12Device->GetDescriptorHandleIncrementSize(HeapType);
}

FDynamicDescriptorHeapD3D12::~FDynamicDescriptorHeapD3D12()
{
}

void FDynamicDescriptorHeapD3D12::CleanupUsedHeaps(uint64 fenceValue)
{
    RetireCurrentHeap();
    RetireUsedHeaps(fenceValue);
    m_GraphicsHandleCache.ClearCache();
    m_ComputeHandleCache.ClearCache();
}

inline ID3D12DescriptorHeap* FDynamicDescriptorHeapD3D12::GetHeapPointer()
{
    if (m_CurrentHeapPtr == nullptr)
    {
        HE_ASSERT(m_CurrentOffset == 0);
        m_CurrentHeapPtr = RequestDescriptorHeap(m_DescriptorType);
        FCpuDescriptorHandle CpuHandle{ m_CurrentHeapPtr->GetCPUDescriptorHandleForHeapStart().ptr };
        FGpuDescriptorHandle GpuHandle{ m_CurrentHeapPtr->GetGPUDescriptorHandleForHeapStart().ptr };
        m_CurrentHeapPtr->GetGPUDescriptorHandleForHeapStart();
        m_FirstDescriptor = FDescriptorHandle(CpuHandle, GpuHandle);
    }

    return m_CurrentHeapPtr;
}

uint32 FDynamicDescriptorHeapD3D12::DescriptorHandleCache::ComputeStagedSize()
{
    // Sum the maximum assigned offsets of stale descriptor tables to determine total needed space.
    uint32 NeededSpace = 0;
    uint32 RootIndex;
    uint32 StaleParams = m_StaleRootParamsBitMap;
    while (_BitScanForward((unsigned long*)&RootIndex, StaleParams))
    {
        StaleParams ^= (1 << RootIndex);

        uint32 MaxSetHandle = 0;
        HE_ASSERT(TRUE == _BitScanReverse((unsigned long*)&MaxSetHandle, m_RootDescriptorTable[RootIndex].AssignedHandlesBitMap)); // Root entry marked as stale but has no stale descriptors.

        NeededSpace += MaxSetHandle + 1;
    }
    return NeededSpace;
}

void FDynamicDescriptorHeapD3D12::DescriptorHandleCache::CopyAndBindStaleTables(
    D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32 DescriptorSize,
    FDescriptorHandle DestHandleStart, ID3D12GraphicsCommandList* CmdList,
    void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE))
{
    uint32 StaleParamCount = 0;
    uint32 TableSize[DescriptorHandleCache::kMaxNumDescriptorTables];
    uint32 RootIndices[DescriptorHandleCache::kMaxNumDescriptorTables];
    uint32 NeededSpace = 0;
    uint32 RootIndex;

    // Sum the maximum assigned offsets of stale descriptor tables to determine total needed space.
    uint32 StaleParams = m_StaleRootParamsBitMap;
    while (_BitScanForward((unsigned long*)&RootIndex, StaleParams))
    {
        RootIndices[StaleParamCount] = RootIndex;
        StaleParams ^= (1 << RootIndex);

        uint32 MaxSetHandle = 0;
        HE_ASSERT(TRUE == _BitScanReverse((unsigned long*)&MaxSetHandle, m_RootDescriptorTable[RootIndex].AssignedHandlesBitMap)); // Root entry marked as stale but has no stale descriptors.

        NeededSpace += MaxSetHandle + 1;
        TableSize[StaleParamCount] = MaxSetHandle + 1;

        ++StaleParamCount;
    }

    HE_ASSERT(StaleParamCount <= DescriptorHandleCache::kMaxNumDescriptorTables); // We're only equipped to handle so many descriptor tables.

    m_StaleRootParamsBitMap = 0;

    static const uint32 kMaxDescriptorsPerCopy = 16;
    uint32 NumDestDescriptorRanges = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE pDestDescriptorRangeStarts[kMaxDescriptorsPerCopy];
    UINT pDestDescriptorRangeSizes[kMaxDescriptorsPerCopy];

    uint32 NumSrcDescriptorRanges = 0;
    D3D12_CPU_DESCRIPTOR_HANDLE pSrcDescriptorRangeStarts[kMaxDescriptorsPerCopy];
    UINT pSrcDescriptorRangeSizes[kMaxDescriptorsPerCopy];

    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
    HE_ASSERT(pID3D12Device != NULL);


    for (uint32 i = 0; i < StaleParamCount; ++i)
    {
        RootIndex = RootIndices[i];
        D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle{ DestHandleStart.GetGpuPtr() };
        (CmdList->*SetFunc)(RootIndex, GpuHandle);

        DescriptorTableCache& RootDescTable = m_RootDescriptorTable[RootIndex];

        D3D12_CPU_DESCRIPTOR_HANDLE* SrcHandles = RootDescTable.TableStart;
        uint64 SetHandles = (uint64)RootDescTable.AssignedHandlesBitMap;
        D3D12_CPU_DESCRIPTOR_HANDLE CurDest{ DestHandleStart.GetCpuPtr() };
        DestHandleStart += TableSize[i] * DescriptorSize;

        unsigned long SkipCount;
        while (_BitScanForward64(&SkipCount, SetHandles))
        {
            // Skip over unset descriptor handles
            SetHandles >>= SkipCount;
            SrcHandles += SkipCount;
            CurDest.ptr += SkipCount * DescriptorSize;

            unsigned long DescriptorCount;
            _BitScanForward64(&DescriptorCount, ~SetHandles);
            SetHandles >>= DescriptorCount;

            // If we run out of temp room, copy what we've got so far
            if (NumSrcDescriptorRanges + DescriptorCount > kMaxDescriptorsPerCopy)
            {
                pID3D12Device->CopyDescriptors(
                    NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
                    NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes,
                    Type);

                NumSrcDescriptorRanges = 0;
                NumDestDescriptorRanges = 0;
            }

            // Setup destination range
            pDestDescriptorRangeStarts[NumDestDescriptorRanges] = CurDest;
            pDestDescriptorRangeSizes[NumDestDescriptorRanges] = DescriptorCount;
            ++NumDestDescriptorRanges;

            // Setup source ranges (one descriptor each because we don't assume they are contiguous)
            for (uint32 j = 0; j < DescriptorCount; ++j)
            {
                pSrcDescriptorRangeStarts[NumSrcDescriptorRanges] = SrcHandles[j];
                pSrcDescriptorRangeSizes[NumSrcDescriptorRanges] = 1;
                ++NumSrcDescriptorRanges;
            }

            // Move the destination pointer forward by the number of descriptors we will copy
            SrcHandles += DescriptorCount;
            CurDest.ptr += DescriptorCount * DescriptorSize;
        }
    }

    pID3D12Device->CopyDescriptors(
        NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
        NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes,
        Type);
}

void FDynamicDescriptorHeapD3D12::CopyAndBindStagedTables(DescriptorHandleCache& HandleCache, ID3D12GraphicsCommandList* CmdList,
    void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE))
{
    uint32 NeededSize = HandleCache.ComputeStagedSize();
    if (!HasSpace(NeededSize))
    {
        RetireCurrentHeap();
        UnbindAllValid();
        NeededSize = HandleCache.ComputeStagedSize();
    }

    // This can trigger the creation of a new heap
    m_OwningContext.SetDescriptorHeap((EResourceHeapType)m_DescriptorType, this);
    HandleCache.CopyAndBindStaleTables(m_DescriptorType, m_DescriptorSize, Allocate(NeededSize), CmdList, SetFunc);
}

void FDynamicDescriptorHeapD3D12::UnbindAllValid(void)
{
    m_GraphicsHandleCache.UnbindAllValid();
    m_ComputeHandleCache.UnbindAllValid();
}

D3D12_GPU_DESCRIPTOR_HANDLE FDynamicDescriptorHeapD3D12::UploadDirect(D3D12_CPU_DESCRIPTOR_HANDLE Handle)
{
    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());
    HE_ASSERT(pID3D12Device != NULL);

    if (!HasSpace(1))
    {
        RetireCurrentHeap();
        UnbindAllValid();
    }

    m_OwningContext.SetDescriptorHeap((EResourceHeapType)m_DescriptorType, this);

    FDescriptorHandle DestHandle = m_FirstDescriptor + m_CurrentOffset * m_DescriptorSize;
    m_CurrentOffset += 1;
    D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle{ DestHandle.GetCpuPtr() };
    D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle{ DestHandle.GetGpuPtr() };
    pID3D12Device->CopyDescriptorsSimple(1, CPUHandle, Handle, m_DescriptorType);

    return GPUHandle;
}

void FDynamicDescriptorHeapD3D12::DescriptorHandleCache::UnbindAllValid()
{
    m_StaleRootParamsBitMap = 0;

    unsigned long TableParams = m_RootDescriptorTablesBitMap;
    unsigned long RootIndex;
    while (_BitScanForward(&RootIndex, TableParams))
    {
        TableParams ^= (1 << RootIndex);
        if (m_RootDescriptorTable[RootIndex].AssignedHandlesBitMap != 0)
            m_StaleRootParamsBitMap |= (1 << RootIndex);
    }
}

void FDynamicDescriptorHeapD3D12::DescriptorHandleCache::StageDescriptorHandles(uint32 RootIndex, uint32 Offset, uint32 NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
{
    HE_ASSERT(((1 << RootIndex) & m_RootDescriptorTablesBitMap) != 0); // Root parameter is not a CBV_SRV_UAV descriptor table.
    HE_ASSERT(Offset + NumHandles <= m_RootDescriptorTable[RootIndex].TableSize);

    DescriptorTableCache& TableCache = m_RootDescriptorTable[RootIndex];
    D3D12_CPU_DESCRIPTOR_HANDLE* CopyDest = TableCache.TableStart + Offset;
    for (uint32 i = 0; i < NumHandles; ++i)
        CopyDest[i] = Handles[i];
    TableCache.AssignedHandlesBitMap |= ((1 << NumHandles) - 1) << Offset;
    m_StaleRootParamsBitMap |= (1 << RootIndex);
}

void FDynamicDescriptorHeapD3D12::DescriptorHandleCache::ParseRootSignature(D3D12_DESCRIPTOR_HEAP_TYPE Type, const FRootSignature& RootSig)
{
    uint32 CurrentOffset = 0;

    HE_ASSERT(RootSig.m_NumParameters <= 16); // Maybe we need to support something greater.

    m_StaleRootParamsBitMap = 0;
    m_RootDescriptorTablesBitMap = (Type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ?
        RootSig.m_SamplerTableBitMap : RootSig.m_DescriptorTableBitMap);

    unsigned long TableParams = m_RootDescriptorTablesBitMap;
    unsigned long RootIndex;
    while (_BitScanForward(&RootIndex, TableParams))
    {
        TableParams ^= (1 << RootIndex);

        uint32 TableSize = RootSig.m_DescriptorTableSize[RootIndex];
        HE_ASSERT(TableSize > 0);

        DescriptorTableCache& RootDescriptorTable = m_RootDescriptorTable[RootIndex];
        RootDescriptorTable.AssignedHandlesBitMap = 0;
        RootDescriptorTable.TableStart = m_HandleCache + CurrentOffset;
        RootDescriptorTable.TableSize = TableSize;

        CurrentOffset += TableSize;
    }

    m_MaxCachedDescriptors = CurrentOffset;

    HE_ASSERT(m_MaxCachedDescriptors <= kMaxNumDescriptors); // Exceeded user-supplied maximum cache size.
}
