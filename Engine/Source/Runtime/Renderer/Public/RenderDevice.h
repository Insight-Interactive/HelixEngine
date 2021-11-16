#pragma once

#include "CoreFwd.h"
#include "RendererFwd.h"
#include "CommonStructs.h"

class FDepthBuffer;
class FRootSignature;
class FPipelineState;
class FDescriptorHeap;
class FColorBuffer;
class HTexture;

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
		const HTexture** pSrcDescriptorRangeStarts,
		const uint32* pSrcDescriptorRangeSizes,
		EResourceHeapType DescriptorHeapsType
	);

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
