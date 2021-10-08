// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContextFactory.h"


using Super = FRenderContextFactory;

class RENDER_API FRenderContextFactoryD3D12 final : public FRenderContextFactory
{
	friend class FRendererInitializer;
public:
	FRenderContextFactoryD3D12();
	virtual ~FRenderContextFactoryD3D12();

	virtual void CreateContext(FRenderContext& OutContext) override;

protected:
	virtual void CreateSwapChain(FSwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, FCommandManager* InCommandManager, FRenderDevice* InDevice) override;
	virtual void CreateDevice(FRenderDevice** OutDevice) override;
	virtual void CreateCommandManager(FCommandManager** OutCommandManager, FRenderDevice* InDevice) override;
	virtual void CreateContextManager(FContextManager** OutCommandContext) override;
	virtual void CreateGeometryManager(FGeometryBufferManager** OutGeometryManager) override;
	virtual void CreateConstantBufferManager(FConstantBufferManager** OutCBManager) override;
	virtual void CreateTextureManager(FTextureManager** OutTexManager) override;

	//
	// DirectX helpers
	//
	void CreateDXGIFactory();

};
