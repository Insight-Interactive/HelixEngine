// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"
#include "RendererFwd.h"
#include "CommonStructs.h"

class FDepthBuffer;
class FRootSignature;
class FPipelineState;
class FDescriptorHeap;
class FColorBuffer;
class FTexture;

struct DeviceInitParams
{
	bool ForceWarpAdapter;
#if R_WITH_D3D12
	bool CheckForDXRSupport;
	D3D_FEATURE_LEVEL MinFeatureLevel;
	D3D_FEATURE_LEVEL MinDXRFeatureLevel;
#endif
};

struct DeviceQueryResult
{
	TChar DeviceName[R_MAX_DEVICE_NAME_LENGTH];
	bool IsRealtimeRTSupported;
};

class RENDER_API FRenderDevice
{
	friend class FRenderContext;
	friend class FRenderContextFactory;
public:
	FRenderDevice();
	~FRenderDevice();

	void* GetNativeDevice() const;

	void CopyDescriptors(
		uint32 NumDestDescriptorRanges,
		const FCpuDescriptorHandle* pDestDescriptorRangeStarts,
		const uint32* pDestDescriptorRangeSizes,
		uint32 NumSrcDescriptorRanges,
		const FTexture** pSrcDescriptorRangeStarts,
		const uint32* pSrcDescriptorRangeSizes,
		EResourceHeapType DescriptorHeapsType
	);
	uint32 GetDescriptorHandleIncrementSize( EResourceHeapType Type );

protected:
	void Initialize(const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult, void** ppFactoryContext);
	void UnInitialize();

private:

#if R_WITH_D3D12
	// Feature Check Functions
	
	// Check for Shader Model 6 support.
	bool CheckSM6Support( ID3D12Device* pDevice );
	// Check for DirectX realtime raytracing support.
	bool CheckDXRSupport( ID3D12Device* pDevice );

	void GetHardwareAdapter( IDXGIFactory6* pFactory, IDXGIAdapter1** ppAdapter, const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult );


	ID3D12Device* m_pD3DDevice;
	uint32 m_DescriptorHandleIncrementSizeCache[RHT_HeapType_Count];

#endif // R_WITH_D3D12

};


// 
// Inline function implementations
//

FORCEINLINE void* FRenderDevice::GetNativeDevice() const
{
#if R_WITH_D3D12
	return (void*)m_pD3DDevice;
#endif
}

FORCEINLINE uint32 FRenderDevice::GetDescriptorHandleIncrementSize(EResourceHeapType Type)
{
	static bool SizesCached = false;
	if (!SizesCached)
	{
		for (uint32 i = 0; i < RHT_HeapType_Count; i++)
		{
			m_DescriptorHandleIncrementSizeCache[i] = m_pD3DDevice->GetDescriptorHandleIncrementSize( (D3D12_DESCRIPTOR_HEAP_TYPE)i );
		}
		SizesCached = true;
	}
	return m_DescriptorHandleIncrementSizeCache[Type];
}
