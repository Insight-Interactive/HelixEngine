// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "GpuResource.h"
#include "CriticalSection.h"


// Constant blocks must be multiples of 16 constants @ 16 bytes each
#define DEFAULT_ALIGN 256

// Various types of allocations may contain NULL pointers.  Check before dereferencing if you are unsure.
struct DynAlloc
{
    DynAlloc(FGpuResource& BaseResource, uint64 ThisOffset, uint64 ThisSize)
        : Buffer(BaseResource)
        , Offset(ThisOffset)
        , Size(ThisSize)
    {
    }

    FGpuResource& Buffer;	// The D3D buffer associated with this memory.
    uint64 Offset;			// Offset from start of buffer resource
    uint64 Size;			// Reserved size of this allocation
    void* DataPtr;			// The CPU-writeable address
    uint64 GpuAddress;	    // The GPU-visible address
};


class FLinearAllocationPage : public FGpuResource
{
public:
    FLinearAllocationPage(void* pResource, EResourceState Usage) 
        : FGpuResource()
    {
#if R_WITH_D3D12
        m_pID3D12Resource.Attach((ID3D12Resource*)pResource);
        m_pID3D12Resource->Map(0, nullptr, &m_CpuVirtualAddress);
        m_GpuVirtualAddress = m_pID3D12Resource->GetGPUVirtualAddress();
#endif
        m_UsageState = Usage;
    }

    ~FLinearAllocationPage()
    {
        Unmap();
    }

    void Map(void)
    {
#if R_WITH_D3D12
        if (m_CpuVirtualAddress == nullptr)
        {
            m_pID3D12Resource->Map(0, nullptr, &m_CpuVirtualAddress);
        }
#endif
    }

    void Unmap(void)
    {
#if R_WITH_D3D12
        if (m_CpuVirtualAddress != nullptr)
        {
            m_pID3D12Resource->Unmap(0, nullptr);
            m_CpuVirtualAddress = nullptr;
        }
#endif
    }

    void*   m_CpuVirtualAddress;
    uint64  m_GpuVirtualAddress;
};

enum ELinearAllocatorType
{
    LAT_InvalidAllocator = -1,

    LAT_GpuExclusive = 0,		// DEFAULT   GPU-writeable (via UAV)
    LAT_CpuWritable = 1,		// UPLOAD CPU-writeable (but write combined)

    LAT_NumAllocatorTypes
};

enum
{
    kGpuAllocatorPageSize = 0x10000,	// 64K
    kCpuAllocatorPageSize = 0x200000	// 2MB
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

    void Destroy() { m_PagePool.clear(); }

private:
    ELinearAllocatorType                                        m_AllocationType;
    std::vector<std::unique_ptr<FLinearAllocationPage> >        m_PagePool;
    std::queue<std::pair<uint64_t, FLinearAllocationPage*> >    m_RetiredPages;
    std::queue<std::pair<uint64_t, FLinearAllocationPage*> >    m_DeletionQueue;
    std::queue<FLinearAllocationPage*>                          m_AvailablePages;
    CriticalSection m_Mutex;

    static ELinearAllocatorType SAutoType;

};

class FLinearAllocator
{
public:

    FLinearAllocator(ELinearAllocatorType Type) : m_AllocationType(Type), m_PageSize(0), m_CurOffset(~(size_t)0), m_CurPage(nullptr)
    {
        HE_ASSERT(Type > LAT_InvalidAllocator && Type < LAT_NumAllocatorTypes);
        m_PageSize = (Type == LAT_GpuExclusive ? kGpuAllocatorPageSize : kCpuAllocatorPageSize);
    }

    DynAlloc Allocate(size_t SizeInBytes, size_t Alignment = DEFAULT_ALIGN);

    void CleanupUsedPages(uint64_t FenceID);

    static void DestroyAll(void)
    {
        SPageManager[0].Destroy();
        SPageManager[1].Destroy();
    }

private:
    DynAlloc AllocateLargePage(size_t SizeInBytes);

private:
    ELinearAllocatorType m_AllocationType;
    size_t m_PageSize;
    size_t m_CurOffset;
    FLinearAllocationPage* m_CurPage;
    std::vector<FLinearAllocationPage*> m_RetiredPages;
    std::vector<FLinearAllocationPage*> m_LargePageList;

    static FLinearAllocatorPageManager SPageManager[2];
};
