#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ISwapChain.h"


class FCommandQueueD3D12;

class RENDER_API FSwapChainD3D12 final : public FSwapChain
{
public:
	FSwapChainD3D12();
	~FSwapChainD3D12();

	virtual void Initialize(FRenderDevice* pDevice) override;

	virtual void SwapBuffers() override;
	virtual void Resize(const uint32& Width, const uint32& Height) override;
	virtual void SetNumBackBuffes(uint32 NumBuffers) override;
	virtual void SetBackBufferFormat(EFormat& Format) override;

	virtual void* GetNativeSwapChain() const { return SCast<void*>(m_pDXGISwapChain); }

	virtual void ToggleFullScreen(bool IsEnabled) override;

	void Create(const FSwapChainDesc& InitParams, IDXGIFactory6** ppDXGIFactory, FCommandQueueD3D12* ppCommandQueue, ID3D12Device* pDevice);


protected:
	virtual void UnInitialize() override;
	//
	// Utility
	//
	void ResizeDXGIBuffers();


	//
	// D3D Initialize
	//
	void BindSwapChainBackBuffers();
	void CheckTearingSupport(IDXGIFactory6* pFactory);

protected:

	ID3D12Device* m_pID3D12DeviceRef;

	IDXGISwapChain3* m_pDXGISwapChain;

};
