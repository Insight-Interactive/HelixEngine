#pragma once

#include "CoreFwd.h"
#include "RendererFwd.h"
#include "CommonStructs.h"

class IDepthBuffer;
class IRootSignature;
class IPipelineState;
class IDescriptorHeap;
class IColorBuffer;
class ITexture;

struct DeviceInitParams
{
	bool ForceWarpAdapter;
};

struct DeviceQueryResult
{
	TChar DeviceName[R_MAX_DEVICE_NAME_LENGTH];
};

class RENDER_API IDevice
{
	friend class RenderContext;
public:
	virtual void* GetNativeDevice() const = 0;

	virtual void CreatePipelineState(const PipelineStateDesc& PSODesc, IPipelineState** ppOutPSO) = 0;
	virtual void CreateRootSignature(IRootSignature** ppOutSignature) = 0;
	virtual void CreateDescriptorHeap(const TChar* DebugHeapName, EResourceHeapType&& Type, uint32&& MaxCount, IDescriptorHeap** ppOutHeap) = 0;
	virtual void CreateColorBuffer(const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format, IColorBuffer** ppOutColorBuffer) = 0;
	virtual void CreateDepthBuffer(const TChar* Name, uint32 Width, uint32 Height, EFormat Format, IDepthBuffer** ppOutDepthBuffer) = 0;
	virtual void CopyDescriptors(
		uint32 NumDestDescriptorRanges,
		const CpuDescriptorHandle* pDestDescriptorRangeStarts,
		const uint32* pDestDescriptorRangeSizes,
		uint32 NumSrcDescriptorRanges,
		const ITexture** pSrcDescriptorRangeStarts,
		const uint32* pSrcDescriptorRangeSizes,
		EResourceHeapType DescriptorHeapsType
	) = 0;

protected:
	IDevice() {}
	virtual ~IDevice() {}

	virtual void Initialize(const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult, void** ppFactoryContext) = 0;
	virtual void UnInitialize() = 0;
};
