#pragma once

#include "Engine/EngineDefines.h"


/*
	Common root parameters that each root signature MUST use.
	If a pass requeres additional root parameters (textures, buffers, etc.)
	their first element must be equal to `kLights + 1`. The order after
	that does not matter and is up to the user.
*/
enum ECommonRootParamsBuffers
{
	kSceneConstants = kSceneConstantsReg,
	kMeshWorld		= kMeshWorldReg,
	kMaterial		= kMaterialReg,
	kLights			= kLightsReg,

	kNumCommonRootParams,
};


//
// Geometry Pass
//
enum EGeometryPassRootParams
{
	GRP_MaterialTextureAlbedo = ECommonRootParamsBuffers::kNumCommonRootParams,
	GRP_MaterialTextureNormal = GRP_MaterialTextureAlbedo + 1,

	GRP_NumParams = 2,
};
enum EGeometryPassDescriptorRanges
{
	GDR_MaterialTextureAlbedo = 0,
	GDR_MaterialTextureNormal = 1,
	//GDR_Texture_Metallic = 2,
	//GDR_Texture_Roughness = 3,

	GDR_RangeCount = 2,
};


//
// Light Pass
//
enum ELightPassRootParams
{
	LRP_GBufferTextureSceneDepth = ECommonRootParamsBuffers::kNumCommonRootParams,
	LRP_GBufferTextureAlbedo = LRP_GBufferTextureSceneDepth + 1,
	LRP_GBufferTextureNormal = LRP_GBufferTextureAlbedo + 1,
	LRP_GBufferTexturePosition = LRP_GBufferTextureNormal + 1,
	//LRP_GBufferTextureMetallic	= 2,
	//LRP_GBufferTextureRoughness	= 3,

	LRP_NumParams = 3,
};
enum ELightPassDescriptorRanges
{
	LDR_GBufferTextureAlbedo = 0,
	LDR_GBufferTextureNormal = 1,
	//GDR_Texture_Metallic = 2,
	//GDR_Texture_Roughness = 3,

	LDR_RangeCount = 2,
};


//
// Sky Pass
//
enum ESkyPassRootParams
{
	SPRP_Diffuse = ECommonRootParamsBuffers::kNumCommonRootParams,
};
enum ESkyPassDEscriptorRanges
{
	SPDR_Diffuse = 0,
};
