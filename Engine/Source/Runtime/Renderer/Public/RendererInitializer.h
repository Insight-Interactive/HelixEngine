#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"


class FRenderContext;
class FSwapChain;

class RENDER_API FRendererInitializer
{
	friend class FRenderingSubsystem;
public:
	static void InitializeSwapChain( FSwapChain& OutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height );

private:
	static void InitializeContext( FRenderContext& Context );
	static void UnInitializeContext( FRenderContext& Context );

};
