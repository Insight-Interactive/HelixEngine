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
};

struct DeviceQueryResult
{
	TChar DeviceName[R_MAX_DEVICE_NAME_LENGTH];
};

class RENDER_API FRenderDevice
{
	friend class FRenderContext;
public:
	virtual void* GetNativeDevice() const = 0;

	virtual void CreatePipelineState(const FPipelineStateDesc& PSODesc, FPipelineState** ppOutPSO) = 0;
	virtual void CreateRootSignature(FRootSignature** ppOutSignature) = 0;
	virtual void CreateDescriptorHeap(const TChar* DebugHeapName, EResourceHeapType&& Type, uint32&& MaxCount, FDescriptorHeap** ppOutHeap) = 0;
	virtual void CreateColorBuffer(const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format, FColorBuffer** ppOutColorBuffer) = 0;
	virtual void CreateDepthBuffer(const TChar* Name, uint32 Width, uint32 Height, EFormat Format, FDepthBuffer** ppOutDepthBuffer) = 0;
	virtual void CopyDescriptors(
		uint32 NumDestDescriptorRanges,
		const FCpuDescriptorHandle* pDestDescriptorRangeStarts,
		const uint32* pDestDescriptorRangeSizes,
		uint32 NumSrcDescriptorRanges,
		const HTexture** pSrcDescriptorRangeStarts,
		const uint32* pSrcDescriptorRangeSizes,
		EResourceHeapType DescriptorHeapsType
	) = 0;

protected:
	FRenderDevice() {}
	virtual ~FRenderDevice() {}

	virtual void Initialize(const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult, void** ppFactoryContext) = 0;
	virtual void UnInitialize() = 0;
};
