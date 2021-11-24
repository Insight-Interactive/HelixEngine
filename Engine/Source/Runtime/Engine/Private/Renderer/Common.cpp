// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "RendererCore.h"
#include "Renderer/Common.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/LightManager.h"

MaterialManager GMaterialManager;
LightManager GLightManager;


FInputElementDesc GSceneMeshInputElements[6] =
{
	{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "NORMAL",		0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "TANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "BITANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "COLOR",		0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,			0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumSceneMeshCommonInputElements = HE_ARRAYSIZE(GSceneMeshInputElements);

FInputElementDesc GScreenSpaceInputElements[2] =
{
	{ "POSITION",	0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumScreenSpaceInputElements = HE_ARRAYSIZE(GScreenSpaceInputElements);


const float kMinTexLOD = 0.0f;
const float kMaxTexLOD = 9.0f;
FSamplerDesc GLinearWrapSamplerDesc
{
	F_Comparison_Min_Mag_Mip_Linear,
	TAM_Wrap,
	TAM_Wrap,
	TAM_Wrap,
	0,
	1,
	CF_LessEqual,
	{ 0, 0, 0, 0 },
	kMinTexLOD,
	kMaxTexLOD,
};
