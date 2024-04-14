#pragma once

#include "Engine/EngineDefines.h"


/*
	Common root parameters that each root signature MUST use.
	If a pass requires additional root parameters (textures, buffers, etc.)
	their first element must be equal to `kSkeletonBonesReg + 1`. The order after
	that does not matter and is up to the user.
*/
enum ECommonRootParamsBuffers
{
	kSceneConstants = kSceneConstantsReg,
	kMeshWorld		= kMeshWorldReg,
	kLights			= kLightsReg,
	kSkeletonBones	= kSkeletonBonesReg,

	kNumCommonRootParams,
};


//
// Geometry Pass
//
enum EGeometryPassRootParams
{
	GRP_MaterialTextureAlbedo = ECommonRootParamsBuffers::kNumCommonRootParams,
	GRP_MaterialTextureNormal = GRP_MaterialTextureAlbedo + 1,
	GRP_MaterialTextureRoughness = GRP_MaterialTextureNormal + 1,
	GRP_MaterialTextureMetallic = GRP_MaterialTextureRoughness + 1,

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
	LRP_GBufferTextureMetallic	= LRP_GBufferTextureNormal + 1,
	LRP_GBufferTextureRoughness	= LRP_GBufferTextureMetallic + 1,
	LRP_GBufferTextureSpecular	= LRP_GBufferTextureRoughness + 1,

	LRP_NumParams = 9,
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
