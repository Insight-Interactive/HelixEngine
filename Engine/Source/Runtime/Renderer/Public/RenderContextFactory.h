#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContext.h"
#include "RendererCore.h"

class FRenderDevice;
class FSwapChain;
class FCommandManager;
class FContextManager;
class FGeometryBufferManager;
class FConstantBufferManager;
class FTextureManager;

class RENDER_API FRenderContextFactory
{
protected:
	virtual void CreateContext(FRenderContext& OutContext) = 0;

	FORCEINLINE void InitializeMainComponents();

	virtual void CreateDevice(FRenderDevice** OutDevice) = 0;
	virtual void CreateSwapChain(FSwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, FCommandManager* InCommandManager, FRenderDevice* InDevice) = 0;
	virtual void CreateCommandManager(FCommandManager** OutCommandManager, FRenderDevice* InDevice) = 0;
	virtual void CreateContextManager(FContextManager** OutCommandContext) = 0;
	virtual void CreateGeometryManager(FGeometryBufferManager** OutGeometryManager) = 0;
	virtual void CreateConstantBufferManager(FConstantBufferManager** OutCBManager) = 0;
	virtual void CreateTextureManager(FTextureManager** OutTexManager) = 0;

protected:
	FRenderContextFactory()
	{
	}

};


//
// Inline function implementations
//

FORCEINLINE void FRenderContextFactory::InitializeMainComponents()
{
	CreateDevice(&GDevice);

	CreateCommandManager(&GCommandManager, GDevice);
	CreateContextManager(&GContextManager);
}
