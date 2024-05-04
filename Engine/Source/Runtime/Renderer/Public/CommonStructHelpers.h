// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererCore.h"
#include "CommonStructs.h"


struct CDepthStencilStateDesc : public FDepthStencilStateDesc
{
	CDepthStencilStateDesc()
	{
		DepthEnable = true;
		DepthWriteMask = DWM_All;
		DepthFunc = CF_Less;
		StencilEnabled = false;
		StencilReadMask = HE_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = HE_DEFAULT_STENCIL_WRITE_MASK;
		const FStencilOpDesc DefaultStencilOp =
		{ SO_Keep, SO_Keep, SO_Keep, CF_Always };
		FrontFace = DefaultStencilOp;
		BackFace = DefaultStencilOp;
	}
};

struct CBlendDesc : public FBlendDesc
{
	CBlendDesc()
	{
		AlphaToCoverageEnable = false;
		IndependentBlendEnable = false;

		FRenderTargetBlendDesc DefaultRenderTargetBlendDesc =
		{
			false, false,
			B_One, B_Zero, BO_Add,
			B_One, B_Zero, BO_Add,
			LO_NoOp,
			CWE_All,
		};
		for (uint32 i = 0; i < HE_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			RenderTarget[i] = DefaultRenderTargetBlendDesc;
	}
};

struct CRasterizerDesc : public FRasterizerDesc
{
	CRasterizerDesc()
	{
		FillMode = FM_Solid;
		CullMode = CM_Back;
		FrontCounterClockwise = false;
		DepthBias = HE_DEFAULT_DEPTH_BIAS;
		DepthBiasClamp = HE_DEFAULT_DEPTH_BIAS_CLAMP;
		SlopeScaledDepthBias = HE_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		DepthClipEnabled = true;
		MultiSampleEnable = false;
		AntialiazedLineEnabled = false;
		ForcedSampleCount = 0;
		ConservativeRaster = CRM_Off;
	}
};
