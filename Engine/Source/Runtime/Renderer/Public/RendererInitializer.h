#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RenderContext.h"


class HEngine;

class RENDER_API RendererInitializer
{
	friend HEngine;
public:
	static void InitializeSwapChain( ISwapChain** pOutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height );

private:
	static void InitializeContext( ERenderBackend API, RenderContext& Context );
	static void UnInitializeContext( RenderContext& Context );

};
