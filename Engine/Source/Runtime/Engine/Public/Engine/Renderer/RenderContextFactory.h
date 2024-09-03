// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContext.h"
#include "RendererCore.h"

class FRenderDevice;
class FSwapChain;
class FCommandManager;
class FContextManager;
class FConstantBufferManager;
class FTextureManager;
class FFontManager;

class RENDER_API FRenderContextFactory
{
public:
	FRenderContextFactory();
	~FRenderContextFactory();

	void CreateContext(FRenderContext& OutContext);
	void CreateSwapChain( FSwapChain& OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, FCommandManager& InCommandManager, FRenderDevice& InDevice );

protected:
	FORCEINLINE void InitializeMainComponents();

	void CreateDevice(FRenderDevice& OutDevice);
	void CreateCommandManager(FCommandManager& OutCommandManager);
	void CreateContextManager(FContextManager& OutCommandContext);
	void CreateTextureManager(FTextureManager& OutTexManager);

private:

#if R_WITH_D3D12
	IDXGIFactory6* m_pDXGIFactory;

	void CreateDXGIFactory();

#endif // R_WITH_D3D12

};


//
// Inline function implementations
//

FORCEINLINE void FRenderContextFactory::InitializeMainComponents()
{
	CreateDevice(GGraphicsDevice);

	CreateCommandManager(GCommandManager);
	CreateContextManager(GContextManager);
}
