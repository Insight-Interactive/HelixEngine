#pragma once

#include "CoreFwd.h"
#include "RendererFwd.h"

#include "IDevice.h"
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

class RENDER_API DeviceD3D12 : public IDevice
{
	friend class RenderContextFactoryD3D12;
public:
	DeviceD3D12();
	virtual ~DeviceD3D12();

	virtual void* GetNativeDevice() const override { return RCast<void*>(m_pD3DDevice); }


	virtual void CreatePipelineState(const PipelineStateDesc& PSODesc, IPipelineState** ppOutPSO) override;
	virtual void CreateRootSignature(IRootSignature** ppOutSignature) override;
	virtual void CreateDescriptorHeap(const TChar* DebugHeapName, EResourceHeapType&& Type, uint32&& MaxCount, IDescriptorHeap** ppOutHeap) override;
	virtual void CreateColorBuffer(const TChar* Name, uint32 Width, uint32 Height, uint32 NumMips, EFormat Format, IColorBuffer** ppOutColorBuffer) override;
	virtual void CreateDepthBuffer(const TChar* Name, uint32 Width, uint32 Height, EFormat Format, IDepthBuffer** ppOutDepthBuffer) override;
	virtual void CopyDescriptors(
		uint32 NumDestDescriptorRanges,
		const CpuDescriptorHandle* pDestDescriptorRangeStarts,
		const uint32* pDestDescriptorRangeSizes,
		uint32 NumSrcDescriptorRanges,
		const ITexture** pSrcDescriptorRangeStarts,
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
