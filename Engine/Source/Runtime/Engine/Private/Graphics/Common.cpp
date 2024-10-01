// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/Renderer/RendererCore.h"
#include "Graphics/Common.h"
#include "Graphics/MaterialManager.h"
#include "Graphics/LightManager.h"

MaterialManager GMaterialManager;
LightManager GLightManager;


FInputElementDesc GStaticMeshInputElements[5] =
{
	{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "NORMAL",		0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "TANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "BITANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,			0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumStaticMeshCommonInputElements = HE_ARRAYSIZE( GStaticMeshInputElements );

/*
	Vertex layout for skinned animated meshes.
*/
FInputElementDesc GSkinnedMeshInputElements[8] =
{
	{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "NORMAL",		0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "TANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "BITANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "COLOR",		0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,			0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "JOINTIDS",	0, F_R32G32B32A32_UInt,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 }, // See H_MAX_JOINTS_PER_VERTEX in EngineDefines.h
	{ "WEIGHTS",	0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumSkinnedMeshCommonInputElements = HE_ARRAYSIZE( GSkinnedMeshInputElements );

FInputElementDesc GScreenSpaceInputElements[2] =
{
	{ "POSITION",	0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumScreenSpaceInputElements = HE_ARRAYSIZE(GScreenSpaceInputElements);

FInputElementDesc GUITextInputElements[3] =
{
	{ "POSITION", 0, F_R32G32B32A32_Float, 0, HE_APPEND_ALIGNED_ELEMENT, IC_PerInstanceData, 1 },
	{ "TEXCOORD",	0, F_R32G32B32A32_Float, 0, HE_APPEND_ALIGNED_ELEMENT, IC_PerInstanceData, 1 },
	{ "COLOR",		0, F_R32G32B32A32_Float, 0, HE_APPEND_ALIGNED_ELEMENT, IC_PerInstanceData, 1 }
};
const uint32 kUITextInputInputElements = HE_ARRAYSIZE( GUITextInputElements );

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
