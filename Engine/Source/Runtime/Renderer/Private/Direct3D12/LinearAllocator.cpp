#include "RendererPCH.h"

#include "LinearAllocator.h"
#include "RendererCore.h"
#include "IDevice.h"
#include "ICommandManager.h"
#include "MathCommon.h"

LinearAllocatorType LinearAllocatorPageManager::sm_AutoType = LAT_GpuExclusive;

LinearAllocatorPageManager::LinearAllocatorPageManager()
{
    m_AllocationType = sm_AutoType;
    sm_AutoType = (LinearAllocatorType)(sm_AutoType + 1);
    HE_ASSERT(sm_AutoType <= LAT_NumAllocatorTypes);
}

LinearAllocatorPageManager LinearAllocator::sm_PageManager[2];

LinearAllocationPage* LinearAllocatorPageManager::RequestPage()
{
    m_Mutex.Enter();

    while (!m_RetiredPages.empty() && GCommandManager->IsFenceComplete(m_RetiredPages.front().first))
    {
        m_AvailablePages.push(m_RetiredPages.front().second);
        m_RetiredPages.pop();
    }

    LinearAllocationPage* PagePtr = nullptr;

    if (!m_AvailablePages.empty())
    {
        PagePtr = m_AvailablePages.front();
        m_AvailablePages.pop();
    }
    else
    {
        PagePtr = CreateNewPage();
        m_PagePool.emplace_back(PagePtr);
    }

    m_Mutex.Exit();
    return PagePtr;
}

void LinearAllocatorPageManager::DiscardPages(uint64_t FenceValue, const std::vector<LinearAllocationPage*>& UsedPages)
{
    m_Mutex.Enter();

    for (auto iter = UsedPages.begin(); iter != UsedPages.end(); ++iter)
        m_RetiredPages.push(std::make_pair(FenceValue, *iter));

    m_Mutex.Exit();
}

void LinearAllocatorPageManager::FreeLargePages(uint64_t FenceValue, const std::vector<LinearAllocationPage*>& LargePages)
{
    m_Mutex.Enter();

    while (!m_DeletionQueue.empty() && GCommandManager->IsFenceComplete(m_DeletionQueue.front().first))
    {
        delete m_DeletionQueue.front().second;
        m_DeletionQueue.pop();
    }

    for (auto iter = LargePages.begin(); iter != LargePages.end(); ++iter)
    {
        (*iter)->Unmap();
        m_DeletionQueue.push(std::make_pair(FenceValue, *iter));
    }
    
    m_Mutex.Exit();
}

LinearAllocationPage* LinearAllocatorPageManager::CreateNewPage(size_t PageSize)
{
    ID3D12Device* pD3D12Device = RCast<ID3D12Device*>(GDevice->GetNativeDevice());

    D3D12_HEAP_PROPERTIES HeapProps;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC ResourceDesc;
    ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResourceDesc.Alignment = 0;
    ResourceDesc.Height = 1;
    ResourceDesc.DepthOrArraySize = 1;
    ResourceDesc.MipLevels = 1;
    ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    ResourceDesc.SampleDesc.Count = 1;
    ResourceDesc.SampleDesc.Quality = 0;
    ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_RESOURCE_STATES DefaultUsage;

    if (m_AllocationType == LAT_GpuExclusive)
    {
        HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        ResourceDesc.Width = PageSize == 0 ? GpuAllocatorPageSize : PageSize;
        ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        DefaultUsage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    }
    else
    {
        HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        ResourceDesc.Width = PageSize == 0 ? CpuAllocatorPageSize : PageSize;
        ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        DefaultUsage = D3D12_RESOURCE_STATE_GENERIC_READ;
    }

    ID3D12Resource* pBuffer;
    pD3D12Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE,
        &ResourceDesc, DefaultUsage, nullptr, IID_PPV_ARGS(&pBuffer));

#if R_DEBUG_GPU_RESOURCES
    pBuffer->SetName(L"LinearAllocator Page");
#endif
    return new LinearAllocationPage(pBuffer, (EResourceState)DefaultUsage);
}

void LinearAllocator::CleanupUsedPages(uint64_t FenceID)
{
    if (m_CurPage == nullptr)
        return;

    m_RetiredPages.push_back(m_CurPage);
    m_CurPage = nullptr;
    m_CurOffset = 0;

    sm_PageManager[m_AllocationType].DiscardPages(FenceID, m_RetiredPages);
    m_RetiredPages.clear();

    sm_PageManager[m_AllocationType].FreeLargePages(FenceID, m_LargePageList);
    m_LargePageList.clear();
}

DynAlloc LinearAllocator::AllocateLargePage(size_t SizeInBytes)
{
    LinearAllocationPage* OneOff = sm_PageManager[m_AllocationType].CreateNewPage(SizeInBytes);
    m_LargePageList.push_back(OneOff);

    DynAlloc ret(*OneOff, 0, SizeInBytes);
    ret.DataPtr = OneOff->m_CpuVirtualAddress;
    ret.GpuAddress = OneOff->m_GpuVirtualAddress;

    return ret;
}

DynAlloc LinearAllocator::Allocate(size_t SizeInBytes, size_t Alignment)
{
    const size_t AlignmentMask = Alignment - 1;

    // Assert that it's a power of two.
    HE_ASSERT((AlignmentMask & Alignment) == 0);

    // Align the allocation
    const size_t AlignedSize = AlignUpWithMask(SizeInBytes, AlignmentMask);

    if (AlignedSize > m_PageSize)
        return AllocateLargePage(AlignedSize);

    m_CurOffset = AlignUp(m_CurOffset, Alignment);

    if (m_CurOffset + AlignedSize > m_PageSize)
    {
        HE_ASSERT(m_CurPage != nullptr);
        m_RetiredPages.push_back(m_CurPage);
        m_CurPage = nullptr;
    }

    if (m_CurPage == nullptr)
    {
        m_CurPage = sm_PageManager[m_AllocationType].RequestPage();
        m_CurOffset = 0;
    }

    DynAlloc ret(*m_CurPage, m_CurOffset, AlignedSize);
    ret.DataPtr = (uint8_t*)m_CurPage->m_CpuVirtualAddress + m_CurOffset;
    ret.GpuAddress = m_CurPage->m_GpuVirtualAddress + m_CurOffset;

    m_CurOffset += AlignedSize;

    SetLastError(0);
    return ret;
}
