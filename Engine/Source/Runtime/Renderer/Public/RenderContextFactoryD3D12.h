// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContextFactory.h"


using Super = IRenderContextFactory;

class RENDER_API RenderContextFactoryD3D12 final : public IRenderContextFactory
{
	friend class RendererInitializer;
public:
	RenderContextFactoryD3D12();
	virtual ~RenderContextFactoryD3D12();

	virtual void CreateContext(RenderContext& OutContext) override;

protected:
	virtual void CreateSwapChain(ISwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, ICommandManager* InCommandManager, IDevice* InDevice) override;
	virtual void CreateDevice(IDevice** OutDevice) override;
	virtual void CreateCommandManager(ICommandManager** OutCommandManager, IDevice* InDevice) override;
	virtual void CreateContextManager(IContextManager** OutCommandContext) override;
	virtual void CreateGeometryManager(IGeometryBufferManager** OutGeometryManager) override;
	virtual void CreateConstantBufferManager(IConstantBufferManager** OutCBManager) override;
	virtual void CreateTextureManager(ITextureManager** OutTexManager) override;

	//
	// DirectX helpers
	//
	void CreateDXGIFactory();

};
