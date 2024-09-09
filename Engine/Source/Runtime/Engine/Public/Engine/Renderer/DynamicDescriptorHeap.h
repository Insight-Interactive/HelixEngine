// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "DescriptorHeap.h"

#include "CriticalSection.h"


class FCommandContext;

class FDynamicDescriptorHeap 
{
public:
    FDynamicDescriptorHeap(FCommandContext& OwningContext, EResourceHeapType HeapType);
    ~FDynamicDescriptorHeap();

    static void DestroyAll(void)
    {
        SDescriptorHeapPool[0].clear();
        SDescriptorHeapPool[1].clear();
    }
    
    FORCEINLINE void* GetNativeHeap() { return (void*)GetHeapPointer(); }

    void CleanupUsedHeaps(uint64 fenceValue);

    // Copy multiple handles into the cache area reserved for the specified root parameter.
    void SetGraphicsDescriptorHandles(uint32 RootIndex, uint32 Offset, uint32 NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
    {
        m_GraphicsHandleCache.StageDescriptorHandles(RootIndex, Offset, NumHandles, Handles);
    }

    void SetComputeDescriptorHandles(uint32 RootIndex, uint32 Offset, uint32 NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
    {
        m_ComputeHandleCache.StageDescriptorHandles(RootIndex, Offset, NumHandles, Handles);
    }

    // Bypass the cache and upload directly to the shader-visible heap
    D3D12_GPU_DESCRIPTOR_HANDLE UploadDirect(D3D12_CPU_DESCRIPTOR_HANDLE Handles);

    // Deduce cache layout needed to support the descriptor tables needed by the root signature.
    void ParseGraphicsRootSignature(const FRootSignature& RootSig)
    {
        m_GraphicsHandleCache.ParseRootSignature(m_DescriptorType, RootSig);
    }

    void ParseComputeRootSignature(const FRootSignature& RootSig)
    {
        m_ComputeHandleCache.ParseRootSignature(m_DescriptorType, RootSig);
    }

    // Upload any new descriptors in the cache to the shader-visible heap.
    inline void CommitGraphicsRootDescriptorTables(void* CmdList)
    {
        if (m_GraphicsHandleCache.m_StaleRootParamsBitMap != 0)
            CopyAndBindStagedTables(m_GraphicsHandleCache, (ID3D12GraphicsCommandList*)CmdList, &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable);
    }

    inline void CommitComputeRootDescriptorTables(void* CmdList)
    {
        if (m_ComputeHandleCache.m_StaleRootParamsBitMap != 0)
            CopyAndBindStagedTables(m_ComputeHandleCache, (ID3D12GraphicsCommandList*)CmdList, &ID3D12GraphicsCommandList::SetComputeRootDescriptorTable);
    }

private:

    // Static members
    static const uint32 kNumDescriptorsPerHeap = 1024;
    static CriticalSection SMutex;
#if R_WITH_D3D12
    static std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> SDescriptorHeapPool[2];
    static std::queue<std::pair<uint64, ID3D12DescriptorHeap*>> SRetiredDescriptorHeaps[2];
    static std::queue<ID3D12DescriptorHeap*> SAvailableDescriptorHeaps[2];
#endif
    
    // Static methods
    static ID3D12DescriptorHeap* RequestDescriptorHeap( EResourceHeapType HeapType);
    static void DiscardDescriptorHeaps( EResourceHeapType HeapType, uint64 FenceValueForReset, const std::vector<ID3D12DescriptorHeap*>& UsedHeaps);

    // Non-static members
    FCommandContext& m_OwningContext;
    ID3D12DescriptorHeap* m_CurrentHeapPtr;
    const EResourceHeapType m_DescriptorType;
    uint32 m_DescriptorSize;
    uint32 m_CurrentOffset;
    FDescriptorHandle m_FirstDescriptor;
    std::vector<ID3D12DescriptorHeap*> m_RetiredHeaps;


    // Describes a descriptor table entry:  a region of the handle cache and which handles have been set
    struct DescriptorTableCache
    {
        DescriptorTableCache() : AssignedHandlesBitMap(0) {}
        uint32 AssignedHandlesBitMap;
        D3D12_CPU_DESCRIPTOR_HANDLE* TableStart;
        uint32 TableSize;
    };

    struct DescriptorHandleCache
    {
        DescriptorHandleCache()
        {
            ClearCache();
        }

        void ClearCache()
        {
            m_RootDescriptorTablesBitMap = 0;
            m_MaxCachedDescriptors = 0;
        }

        uint32 m_RootDescriptorTablesBitMap = 0;
        uint32 m_StaleRootParamsBitMap = 0;
        uint32 m_MaxCachedDescriptors = 0;

        static const uint32 kMaxNumDescriptors = 256;
        static const uint32 kMaxNumDescriptorTables = 16;

        uint32 ComputeStagedSize();
        void CopyAndBindStaleTables( EResourceHeapType Type, uint32 DescriptorSize, FDescriptorHandle DestHandleStart, ID3D12GraphicsCommandList* CmdList,
            void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

        DescriptorTableCache m_RootDescriptorTable[kMaxNumDescriptorTables];
        D3D12_CPU_DESCRIPTOR_HANDLE m_HandleCache[kMaxNumDescriptors];

        void UnbindAllValid();
        void StageDescriptorHandles(uint32 RootIndex, uint32 Offset, uint32 NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[]);
        void ParseRootSignature( EResourceHeapType Type, const FRootSignature& RootSig);
    };

    DescriptorHandleCache m_GraphicsHandleCache;
    DescriptorHandleCache m_ComputeHandleCache;

    bool HasSpace(uint32 Count)
    {
        return (m_CurrentHeapPtr != nullptr && m_CurrentOffset + Count <= kNumDescriptorsPerHeap);
    }

    void RetireCurrentHeap(void);
    void RetireUsedHeaps(uint64 fenceValue);
    void* GetHeapPointer();

    FDescriptorHandle Allocate(uint32 Count)
    {
        FDescriptorHandle ret = m_FirstDescriptor + m_CurrentOffset * m_DescriptorSize;
        m_CurrentOffset += Count;
        return ret;
    }

    void CopyAndBindStagedTables(DescriptorHandleCache& HandleCache, ID3D12GraphicsCommandList* CmdList,
        void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

    // Mark all descriptors in the cache as stale and in need of re-uploading.
    void UnbindAllValid(void);
};
