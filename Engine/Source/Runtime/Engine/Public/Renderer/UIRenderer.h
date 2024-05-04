// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Common.h"
#include "Renderer/FontManager.h"
#include "Color.h"

struct FUIRendererInitParams
{
	FVector2 RenderingResolution;
	EFormat BackBufferFormat;
};

class FViewportContext;
class FUIPanel;

class FUIRenderer
{
public:
	FUIRenderer();
	~FUIRenderer();

	void Initialize( const FUIRendererInitParams& InitParams, FViewportContext& OwningViewport );
	void UnInitialize();

	void RenderPanel( FUIPanel& Panel, FColorBuffer& RenderTarget, FDepthBuffer& DepthBuffer, const FViewPort& View, const FRect& Scissor, uint32 SwapchainFrameIndex );

protected:
	FViewportContext* m_pOwningViewport;

};
