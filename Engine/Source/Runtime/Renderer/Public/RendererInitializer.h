#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContext.h"


class HEngine;

class RENDER_API FRendererInitializer
{
	friend HEngine;
public:
	static void InitializeSwapChain( FSwapChain** pOutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height );

private:
	static void InitializeContext( ERenderBackend API, FRenderContext& Context );
	static void UnInitializeContext( FRenderContext& Context );

};
