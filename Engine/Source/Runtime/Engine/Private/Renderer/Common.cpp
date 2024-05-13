// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "RendererCore.h"
#include "Renderer/Common.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/LightManager.h"

MaterialManager GMaterialManager;
LightManager GLightManager;


/*
	This file contains common vertex layouts for mesh rendering.

	WARNING! Making changes to this file including, parameter reorginization and modification could have influences on the fallowing files:
	Engine/Renderer/Material.cpp
	Renderer/ModelManager.cpp
	Various vertex shaders in the Shaders module
*/

/*
	Vertex layout for static non-animated meshes.
*/
FInputElementDesc GStaticMeshInputElements[6] =
{
	{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "NORMAL",		0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "TANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "BITANGENT",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "COLOR",		0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
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
	{ "JOINTIDS",	0, F_R32G32B32A32_UInt,		0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 }, // See R_MAX_JOINTS_PER_VERTEX in RendererFwd.h
	{ "WEIGHTS",	0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumSkinnedMeshCommonInputElements = HE_ARRAYSIZE( GSkinnedMeshInputElements );


/*
	Vertex layout for screenspace verticies.
*/
FInputElementDesc GScreenSpaceInputElements[2] =
{
	{ "POSITION",	0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	{ "UVS",		0, F_R32G32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
};
const uint32 kNumScreenSpaceInputElements = HE_ARRAYSIZE(GScreenSpaceInputElements);

/*
	Vertex layout for ui element verticies.
*/
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
