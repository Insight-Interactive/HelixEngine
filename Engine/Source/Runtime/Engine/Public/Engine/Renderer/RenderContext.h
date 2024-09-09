// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"


class FSwapChain;
class FRenderDevice;

extern FRenderDevice GGraphicsDevice;

/*
	Responsible for handling rendering related messaged and resource management.
*/
class FRenderContext
{
	friend class FRenderContextFactory;
	friend class FRenderContextFactoryD3D12;
	friend class FRendererInitializer;
public:
	FRenderContext()
	{
	}
	virtual ~FRenderContext()
	{
	}

	virtual void PreFrame();
	virtual void SubmitFrame();
	void PostFrame();


	void OnWindowWindowedModeEntered();
	void OnWindowBorderlessModeEntered();
	void OnNativeResolutionChanged(const uint32& Width, const uint32& height);

protected:
	virtual void Initialize();
	virtual void UnInitialize();

};
