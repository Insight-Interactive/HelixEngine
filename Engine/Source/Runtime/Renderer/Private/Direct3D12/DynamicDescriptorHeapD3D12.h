#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IDescriptorHeap.h"

#include "CriticalSection.h"

class CommandContextD3D12;

class RENDER_API DynamicDescriptorHeapD3D12 : public IDescriptorHeap
{
public:
    DynamicDescriptorHeapD3D12(CommandContextD3D12& OwningContext, D3D12_DESCRIPTOR_HEAP_TYPE HeapType);
    ~DynamicDescriptorHeapD3D12();

    static void DestroyAll(void)
    {
        sm_DescriptorHeapPool[0].clear();
        sm_DescriptorHeapPool[1].clear();
    }

    virtual void* GetNativeHeap() override { return RCast<void*>(GetHeapPointer()); }

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
    void ParseGraphicsRootSignature(const IRootSignature& RootSig)
    {
        m_GraphicsHandleCache.ParseRootSignature(m_DescriptorType, RootSig);
    }

    void ParseComputeRootSignature(const IRootSignature& RootSig)
    {
        m_ComputeHandleCache.ParseRootSignature(m_DescriptorType, RootSig);
    }

    // Upload any new descriptors in the cache to the shader-visible heap.
    inline void CommitGraphicsRootDescriptorTables(ID3D12GraphicsCommandList* CmdList)
    {
        if (m_GraphicsHandleCache.m_StaleRootParamsBitMap != 0)
            CopyAndBindStagedTables(m_GraphicsHandleCache, CmdList, &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable);
    }

    inline void CommitComputeRootDescriptorTables(ID3D12GraphicsCommandList* CmdList)
    {
        if (m_ComputeHandleCache.m_StaleRootParamsBitMap != 0)
            CopyAndBindStagedTables(m_ComputeHandleCache, CmdList, &ID3D12GraphicsCommandList::SetComputeRootDescriptorTable);
    }

private:

    // Static members
    static const uint32 kNumDescriptorsPerHeap = 1024;
    static CriticalSection sm_Mutex;
    static std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> sm_DescriptorHeapPool[2];
    static std::queue<std::pair<uint64, ID3D12DescriptorHeap*>> sm_RetiredDescriptorHeaps[2];
    static std::queue<ID3D12DescriptorHeap*> sm_AvailableDescriptorHeaps[2];

    // Static methods
    static ID3D12DescriptorHeap* RequestDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE HeapType);
    static void DiscardDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE HeapType, uint64 FenceValueForReset, const std::vector<ID3D12DescriptorHeap*>& UsedHeaps);
    virtual void Create(const TChar* DebugHeapName, EResourceHeapType Type, uint32 MaxCount) override { /* Intentionally Left Blank */ }

    // Non-static members
    CommandContextD3D12& m_OwningContext;
    ID3D12DescriptorHeap* m_CurrentHeapPtr;
    const D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorType;
    uint32 m_DescriptorSize;
    uint32 m_CurrentOffset;
    DescriptorHandle m_FirstDescriptor;
    std::vector<ID3D12DescriptorHeap*> m_RetiredHeaps;

    // DONT CALL.
    virtual DescriptorHandle Alloc(uint32 Count) override { return DescriptorHandle{}; }


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
        void CopyAndBindStaleTables(D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32 DescriptorSize, DescriptorHandle DestHandleStart, ID3D12GraphicsCommandList* CmdList,
            void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

        DescriptorTableCache m_RootDescriptorTable[kMaxNumDescriptorTables];
        D3D12_CPU_DESCRIPTOR_HANDLE m_HandleCache[kMaxNumDescriptors];

        void UnbindAllValid();
        void StageDescriptorHandles(uint32 RootIndex, uint32 Offset, uint32 NumHandles, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[]);
        void ParseRootSignature(D3D12_DESCRIPTOR_HEAP_TYPE Type, const IRootSignature& RootSig);
    };

    DescriptorHandleCache m_GraphicsHandleCache;
    DescriptorHandleCache m_ComputeHandleCache;

    bool HasSpace(uint32 Count)
    {
        return (m_CurrentHeapPtr != nullptr && m_CurrentOffset + Count <= kNumDescriptorsPerHeap);
    }

    void RetireCurrentHeap(void);
    void RetireUsedHeaps(uint64 fenceValue);
    ID3D12DescriptorHeap* GetHeapPointer();

    DescriptorHandle Allocate(uint32 Count)
    {
        DescriptorHandle ret = m_FirstDescriptor + m_CurrentOffset * m_DescriptorSize;
        m_CurrentOffset += Count;
        return ret;
    }

    void CopyAndBindStagedTables(DescriptorHandleCache& HandleCache, ID3D12GraphicsCommandList* CmdList,
        void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* SetFunc)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

    // Mark all descriptors in the cache as stale and in need of re-uploading.
    void UnbindAllValid(void);
};
