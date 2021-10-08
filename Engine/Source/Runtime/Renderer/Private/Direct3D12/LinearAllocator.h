#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "GpuResourceD3D12.h"
#include "CriticalSection.h"


// Constant blocks must be multiples of 16 constants @ 16 bytes each
#define DEFAULT_ALIGN 256

// Various types of allocations may contain NULL pointers.  Check before dereferencing if you are unsure.
struct DynAlloc
{
    DynAlloc(FGpuResourceD3D12& BaseResource, uint64 ThisOffset, uint64 ThisSize)
        : Buffer(BaseResource)
        , Offset(ThisOffset)
        , Size(ThisSize)
    {
    }

    FGpuResourceD3D12& Buffer;	// The D3D buffer associated with this memory.
    uint64 Offset;			// Offset from start of buffer resource
    uint64 Size;			// Reserved size of this allocation
    void* DataPtr;			// The CPU-writeable address
    D3D12_GPU_VIRTUAL_ADDRESS GpuAddress;	// The GPU-visible address
};


class FLinearAllocationPage : public FGpuResourceD3D12
{
public:
    FLinearAllocationPage(ID3D12Resource* pResource, EResourceState Usage) 
        : FGpuResourceD3D12()
    {
        m_pID3D12Resource.Attach(pResource);
        m_UsageState = Usage;
        m_GpuVirtualAddress = m_pID3D12Resource->GetGPUVirtualAddress();
        m_pID3D12Resource->Map(0, nullptr, &m_CpuVirtualAddress);
    }

    ~FLinearAllocationPage()
    {
        Unmap();
    }

    void Map(void)
    {
        if (m_CpuVirtualAddress == nullptr)
        {
            m_pID3D12Resource->Map(0, nullptr, &m_CpuVirtualAddress);
        }
    }

    void Unmap(void)
    {
        if (m_CpuVirtualAddress != nullptr)
        {
            m_pID3D12Resource->Unmap(0, nullptr);
            m_CpuVirtualAddress = nullptr;
        }
    }

    void* m_CpuVirtualAddress;
    D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;
};

enum LinearAllocatorType
{
    LAT_InvalidAllocator = -1,

    LAT_GpuExclusive = 0,		// DEFAULT   GPU-writeable (via UAV)
    LAT_CpuWritable = 1,		// UPLOAD CPU-writeable (but write combined)

    LAT_NumAllocatorTypes
};

enum
{
    GpuAllocatorPageSize = 0x10000,	// 64K
    CpuAllocatorPageSize = 0x200000	// 2MB
};

class FLinearAllocatorPageManager
{
public:

    FLinearAllocatorPageManager();
    FLinearAllocationPage* RequestPage(void);
    FLinearAllocationPage* CreateNewPage(size_t PageSize = 0);

    // Discarded pages will get recycled.  This is for fixed size pages.
    void DiscardPages(uint64_t FenceID, const std::vector<FLinearAllocationPage*>& Pages);

    // Freed pages will be destroyed once their fence has passed.  This is for single-use,
    // "large" pages.
    void FreeLargePages(uint64_t FenceID, const std::vector<FLinearAllocationPage*>& Pages);

    void Destroy(void) { m_PagePool.clear(); }

private:

    static LinearAllocatorType sm_AutoType;

    LinearAllocatorType m_AllocationType;
    std::vector<std::unique_ptr<FLinearAllocationPage> > m_PagePool;
    std::queue<std::pair<uint64_t, FLinearAllocationPage*> > m_RetiredPages;
    std::queue<std::pair<uint64_t, FLinearAllocationPage*> > m_DeletionQueue;
    std::queue<FLinearAllocationPage*> m_AvailablePages;
    CriticalSection m_Mutex;
};

class FLinearAllocator
{
public:

    FLinearAllocator(LinearAllocatorType Type) : m_AllocationType(Type), m_PageSize(0), m_CurOffset(~(size_t)0), m_CurPage(nullptr)
    {
        HE_ASSERT(Type > LAT_InvalidAllocator && Type < LAT_NumAllocatorTypes);
        m_PageSize = (Type == LAT_GpuExclusive ? GpuAllocatorPageSize : CpuAllocatorPageSize);
    }

    DynAlloc Allocate(size_t SizeInBytes, size_t Alignment = DEFAULT_ALIGN);

    void CleanupUsedPages(uint64_t FenceID);

    static void DestroyAll(void)
    {
        sm_PageManager[0].Destroy();
        sm_PageManager[1].Destroy();
    }

private:

    DynAlloc AllocateLargePage(size_t SizeInBytes);

    static FLinearAllocatorPageManager sm_PageManager[2];

    LinearAllocatorType m_AllocationType;
    size_t m_PageSize;
    size_t m_CurOffset;
    FLinearAllocationPage* m_CurPage;
    std::vector<FLinearAllocationPage*> m_RetiredPages;
    std::vector<FLinearAllocationPage*> m_LargePageList;
};
