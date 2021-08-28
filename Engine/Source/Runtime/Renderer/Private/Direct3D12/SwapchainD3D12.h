#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ISwapChain.h"


class CommandQueueD3D12;

class RENDER_API SwapChainD3D12 final : public ISwapChain
{
public:
	SwapChainD3D12();
	~SwapChainD3D12();

	virtual void Initialize(IDevice* pDevice) override;

	virtual void SwapBuffers() override;
	virtual void Resize(const uint32& Width, const uint32& Height) override;
	virtual void SetNumBackBuffes(uint32 NumBuffers) override;
	virtual void SetBackBufferFormat(EFormat& Format) override;

	virtual void* GetNativeSwapChain() const { return SCast<void*>(m_pDXGISwapChain); }

	virtual void ToggleFullScreen(bool IsEnabled) override;

	void Create(const SwapChainDescription& InitParams, IDXGIFactory6** ppDXGIFactory, CommandQueueD3D12* ppCommandQueue, ID3D12Device* pDevice);


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
