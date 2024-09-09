// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"


class FRenderContext;
class FSwapChain;

class FRendererInitializer
{
	friend class HEngine;
public:
	static void InitializeSwapChain( FSwapChain& OutSwapChain, void* pNativeWindow, uint32 Width, uint32 Height );

private:
	static void InitializeContext( FRenderContext& Context );
	static void UnInitializeContext( FRenderContext& Context );

};
