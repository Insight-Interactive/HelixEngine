#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContext.h"
#include "RendererCore.h"

class IDevice;
class ISwapChain;
class ICommandManager;
class IContextManager;
class IGeometryBufferManager;
class IConstantBufferManager;
class ITextureManager;

class RENDER_API IRenderContextFactory
{
protected:
	virtual void CreateContext(RenderContext& OutContext) = 0;

	FORCEINLINE void InitializeMainComponents();

	virtual void CreateDevice(IDevice** OutDevice) = 0;
	virtual void CreateSwapChain(ISwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, ICommandManager* InCommandManager, IDevice* InDevice) = 0;
	virtual void CreateCommandManager(ICommandManager** OutCommandManager, IDevice* InDevice) = 0;
	virtual void CreateContextManager(IContextManager** OutCommandContext) = 0;
	virtual void CreateGeometryManager(IGeometryBufferManager** OutGeometryManager) = 0;
	virtual void CreateConstantBufferManager(IConstantBufferManager** OutCBManager) = 0;
	virtual void CreateTextureManager(ITextureManager** OutTexManager) = 0;

protected:
	IRenderContextFactory()
	{
		CreateLogger(GRendererLogger, "Renderer");
	}

};


//
// Inline function implementations
//

FORCEINLINE void IRenderContextFactory::InitializeMainComponents()
{
	CreateDevice(&GDevice);

	CreateCommandManager(&GCommandManager, GDevice);
	CreateContextManager(&GContextManager);
}
