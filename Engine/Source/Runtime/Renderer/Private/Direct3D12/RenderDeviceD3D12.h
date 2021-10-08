#pragma once

#include "CoreFwd.h"
#include "RendererFwd.h"

#include "IRenderDevice.h"
#include "../D3DCommon/D3DCommon.h"

struct DeviceInitParamsD3D12 : public DeviceInitParams
{
	bool CheckForDXRSupport;
	D3D_FEATURE_LEVEL MinFeatureLevel;
	D3D_FEATURE_LEVEL MinDXRFeatureLevel;
};

struct DeviceQueryResultD3D12 : public DeviceQueryResult
{
	bool IsDXRSupported;
};

class RENDER_API FRenderDeviceD3D12 : public FRenderDevice
{
	friend class FRenderContextFactoryD3D12;
public:
	FRenderDeviceD3D12();
	virtual ~FRenderDeviceD3D12();

	virtual void* GetNativeDevice() const override { return RCast<void*>(m_pD3DDevice); }


	virtual void CreatePipelineState(const FPipelineStateDesc& PSODesc, FPipelineState** ppOutPSO) override;
	virtual void CreateRootSignature(FRootSignature** ppOutSignature) override;
	virtual void CreateDescriptorHeap(const TChar* DebugHeapName, EResourceHeapType&& Type, uint32&& MaxCount, FDescriptorHeap** ppOutHeap) override;
	virtual void CreateColorBuffer(const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format, FColorBuffer** ppOutColorBuffer) override;
	virtual void CreateDepthBuffer(const TChar* Name, uint32 Width, uint32 Height, EFormat Format, FDepthBuffer** ppOutDepthBuffer) override;
	virtual void CopyDescriptors(
		uint32 NumDestDescriptorRanges,
		const FCpuDescriptorHandle* pDestDescriptorRangeStarts,
		const uint32* pDestDescriptorRangeSizes,
		uint32 NumSrcDescriptorRanges,
		const HTexture** pSrcDescriptorRangeStarts,
		const uint32* pSrcDescriptorRangeSizes,
		EResourceHeapType DescriptorHeapsType
	) override;

protected:
	void GetHardwareAdapter(IDXGIFactory6* pFactory, IDXGIAdapter1** ppAdapter, const DeviceInitParamsD3D12& InitParams, DeviceQueryResultD3D12& OutDeviceQueryResult);

	//
	// Virtual Functions
	//
	virtual void UnInitialize() override;
	virtual void Initialize(const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult, void** ppFactoryContext) override;

	//
	// Feature Check Functions
	// 
	bool CheckSM6Support(ID3D12Device* pDevice);
	bool CheckDXRSupport(ID3D12Device* pDevice);

private:
	ID3D12Device* m_pD3DDevice;

};
