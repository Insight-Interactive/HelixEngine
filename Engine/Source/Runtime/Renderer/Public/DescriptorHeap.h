#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

// This handle refers to a descriptor or a descriptor table (contiguous descriptors) that is shader visible.
class FDescriptorHandle
{
public:
    FDescriptorHandle()
    {
        m_CpuHandle.Ptr = HE_INVALID_GPU_ADDRESS;
        m_GpuHandle.Ptr = HE_INVALID_GPU_ADDRESS;
    }

    /*
    // Should we allow constructing handles that might not be shader visible?
    FDescriptorHandle( D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle )
        : m_CpuHandle(CpuHandle)
    {
        m_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }
    */

    FDescriptorHandle(FCpuDescriptorHandle CpuHandle, FGpuDescriptorHandle GpuHandle)
        : m_CpuHandle(CpuHandle), m_GpuHandle(GpuHandle)
    {
    }

    FDescriptorHandle operator+ (int32 OffsetScaledByDescriptorSize) const
    {
        FDescriptorHandle ret = *this;
        ret += OffsetScaledByDescriptorSize;
        return ret;
    }

    void operator += (int32 OffsetScaledByDescriptorSize)
    {
        if (m_CpuHandle.Ptr != HE_INVALID_GPU_ADDRESS)
            m_CpuHandle.Ptr += OffsetScaledByDescriptorSize;
        if (m_GpuHandle.Ptr != HE_INVALID_GPU_ADDRESS)
            m_GpuHandle.Ptr += OffsetScaledByDescriptorSize;
    }

    const FCpuDescriptorHandle* operator&() const { return &m_CpuHandle; }
    operator FCpuDescriptorHandle() const { return m_CpuHandle; }
    operator FGpuDescriptorHandle() const { return m_GpuHandle; }

    size_t GetCpuPtr() const { return m_CpuHandle.Ptr; }
    uint64 GetGpuPtr() const { return m_GpuHandle.Ptr; }
    bool IsNull() const { return m_CpuHandle.Ptr == HE_INVALID_GPU_ADDRESS; }
    bool IsShaderVisible() const { return m_GpuHandle.Ptr != HE_INVALID_GPU_ADDRESS; }

private:
    FCpuDescriptorHandle m_CpuHandle;
    FGpuDescriptorHandle m_GpuHandle;
};


class RENDER_API FDescriptorHeap
{
public:
    FDescriptorHeap()
    {
    }
    ~FDescriptorHeap()
    {
        Destroy();
    }

    void* GetNativeHeap();


    void Create( const WChar* DebugName, EResourceHeapType Type, uint32 MaxCount );
    void Destroy( void ) { m_Heap = nullptr; }

    bool HasAvailableSpace( uint32_t Count ) const;
    FDescriptorHandle Alloc( uint32 Count = 1 );

    FDescriptorHandle operator[] ( uint32_t arrayIdx ) const;

    uint32_t GetOffsetOfHandle( const FDescriptorHandle& DHandle );
    bool ValidateHandle( const FDescriptorHandle& DHandle ) const;

    ID3D12DescriptorHeap* GetHeapPointer() const { return m_Heap.Get(); }

    uint32_t GetDescriptorSize( void ) const { return m_DescriptorSize; }

private:
    uint32_t m_DescriptorSize;
    uint32_t m_NumFreeDescriptors;
    FDescriptorHandle m_FirstHandle;
    FDescriptorHandle m_NextFreeHandle;
    
private:
#if R_WITH_D3D12
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_Heap;
    D3D12_DESCRIPTOR_HEAP_DESC m_HeapDesc;
    
#endif

};


// 
// Inline function implementations
//

FORCEINLINE void* FDescriptorHeap::GetNativeHeap()
{ 
    return (ID3D12DescriptorHeap*)m_Heap.Get();
}

FORCEINLINE FDescriptorHandle FDescriptorHeap::operator[] ( uint32_t arrayIdx ) const
{ 
    return m_FirstHandle + arrayIdx * m_DescriptorSize; 
}

FORCEINLINE bool FDescriptorHeap::HasAvailableSpace( uint32_t Count ) const
{ 
    return Count <= m_NumFreeDescriptors; 
}

FORCEINLINE uint32_t FDescriptorHeap::GetOffsetOfHandle( const FDescriptorHandle& DHandle ) 
{
    return (uint32_t)(DHandle.GetCpuPtr() - m_FirstHandle.GetCpuPtr()) / m_DescriptorSize;
}
