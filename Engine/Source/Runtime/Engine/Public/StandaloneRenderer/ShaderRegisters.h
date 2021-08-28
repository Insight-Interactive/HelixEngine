#pragma once


/*
	Common root parameters that each root signature MUST use.
	If a pass requeres additional root parameters (textures, buffers, etc.)
	their first element must be equal to `kNumCommonRootParams`. The order after
	that does not matter and is up to the user.
*/
enum ECommonRootParamsBuffers
{
	kSceneConstants = 0,
	kMeshWorld = 1,
	kMaterial = 2,
	kLights = 3,

	kNumCommonRootParams = kLights + 1,
};


//
// Geometry Pass
//
enum EGeometryPassRootParams
{
	GRP_MaterialTextureAlbedo = kLights + 1,
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
	LRP_GBufferTextureSceneDepth = kLights + 1,
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
	SPRP_Diffuse = kLights + 1,
};
enum ESkyPassDEscriptorRanges
{
	SPDR_Diffuse = 0,
};
