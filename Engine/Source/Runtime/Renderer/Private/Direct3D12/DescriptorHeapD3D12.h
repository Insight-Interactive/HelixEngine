#pragma once

#include "RendererCore.h"

#include "IDescriptorHeap.h"


class RENDER_API FDescriptorHeapD3D12 : public FDescriptorHeap
{
public:
    FDescriptorHeapD3D12() 
    {
    }
    ~FDescriptorHeapD3D12() 
    {
        Destroy();
    }

    virtual void* GetNativeHeap() override { return RCast<ID3D12DescriptorHeap*>(m_Heap.Get()); }


    virtual void Create(const WChar* DebugHeapName, EResourceHeapType Type, uint32 MaxCount) override;
    void Destroy(void) { m_Heap = nullptr; }

    bool HasAvailableSpace(uint32_t Count) const { return Count <= m_NumFreeDescriptors; }
    virtual FDescriptorHandle Alloc(uint32 Count = 1) override;

    FDescriptorHandle operator[] (uint32_t arrayIdx) const { return m_FirstHandle + arrayIdx * m_DescriptorSize; }

    uint32_t GetOffsetOfHandle(const FDescriptorHandle& DHandle) {
        return (uint32_t)(DHandle.GetCpuPtr() - m_FirstHandle.GetCpuPtr()) / m_DescriptorSize;
    }

    bool ValidateHandle(const FDescriptorHandle& DHandle) const;

    ID3D12DescriptorHeap* GetHeapPointer() const { return m_Heap.Get(); }

    uint32_t GetDescriptorSize(void) const { return m_DescriptorSize; }

private:

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_Heap;
    D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
    uint32_t m_DescriptorSize;
    uint32_t m_NumFreeDescriptors;
    FDescriptorHandle m_FirstHandle;
    FDescriptorHandle m_NextFreeHandle;
};