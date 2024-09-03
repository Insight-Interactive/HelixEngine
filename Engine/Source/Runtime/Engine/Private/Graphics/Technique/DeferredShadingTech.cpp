// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Graphics/Technique/DeferredShadingTech.h"

#include "Graphics/ShaderRegisters.h"
#include "Graphics/Common.h"
#include "DataTypeWrappers.h"
#include "FileSystem.h"
#include "Graphics/SceneRenderer.h"
#include "Engine/ViewportContext.h"


//
// Deferred Shading Tech Implementation
//

FDeferredShadingTech::FDeferredShadingTech()
	: m_LightPass( m_GeometryPass )
{

}

FDeferredShadingTech::~FDeferredShadingTech()
{

}

void FDeferredShadingTech::Initialize( FVector2 RenderResolution, EFormat SwapchainFormat )
{
	m_GeometryPass.Initialize( RenderResolution );
	m_LightPass.Initialize( RenderResolution, SwapchainFormat );
}

void FDeferredShadingTech::UnInitialize()
{
	m_GeometryPass.UnInitialize();
	m_LightPass.UnInitialize();
}

//
// Geometry Pass Implementation
//

FDeferredShadingTech::GeometryPass::GeometryPass()
	: m_pSceneDepthBufferRef( NULL )
{

}

FDeferredShadingTech::GeometryPass::~GeometryPass()
{

}

void FDeferredShadingTech::GeometryPass::Initialize( const FVector2& RenderResolution )
{
	HE_ASSERT( m_pSceneDepthBufferRef != NULL ); // Trying to initialize geometry pass with no scene depth buffer! Call 'GeometryPass::SetDepthBuffer' before initilializing.

	m_RS.Reset( 7, 1 );
	m_RS.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	m_RS[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RS[kMeshWorld].InitAsConstantBuffer( kMeshWorld, SV_Vertex );
	m_RS[kLights].InitAsConstantBuffer( kLights, SV_Pixel );
	// Pipeline
	// Albedo
	m_RS[GRP_MaterialTextureAlbedo].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureAlbedo].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	// Normal
	m_RS[GRP_MaterialTextureNormal].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureNormal].SetTableRange( 0, DRT_ShaderResourceView, 1, 1 );
	// Roughness
	m_RS[GRP_MaterialTextureRoughness].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureRoughness].SetTableRange( 0, DRT_ShaderResourceView, 2, 1 );
	// Metallic
	m_RS[GRP_MaterialTextureMetallic].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureMetallic].SetTableRange( 0, DRT_ShaderResourceView, 3, 1 );
	m_RS.Finalize( L"[Deferred Renderer][Geometry Pass] RootSig", RSF_AllowInputAssemblerLayout );


	// Create the render targets.
	//
	Resize( (uint32)RenderResolution.x, (uint32)RenderResolution.y );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultGeometryPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultGeometryPass.ps.cso" );


	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements = kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = CDepthStencilStateDesc();
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = GB_NumBuffers;
	for (uint32 i = 0; i < GB_NumBuffers; ++i)
	{
		PSODesc.RTVFormats[i] = m_RenderTargets[i].GetFormat();
	}
	PSODesc.DSVFormat = m_pSceneDepthBufferRef->GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
}

void FDeferredShadingTech::GeometryPass::UnInitialize()
{
}


void FDeferredShadingTech::GeometryPass::ClearGBuffers( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GfxContext.ClearColorBuffer( GetGBufferColorBuffer( GB_Albedo ), Viewrect );
	GfxContext.ClearColorBuffer( GetGBufferColorBuffer( GB_Normal ), Viewrect );
	GfxContext.ClearColorBuffer( GetGBufferColorBuffer( GB_Roughness ), Viewrect );
	GfxContext.ClearColorBuffer( GetGBufferColorBuffer( GB_Metallic ), Viewrect );
	GfxContext.ClearColorBuffer( GetGBufferColorBuffer( GB_Specular ), Viewrect );
	GfxContext.ClearDepth( *m_pSceneDepthBufferRef );
}

void FDeferredShadingTech::GeometryPass::Bind( FCommandContext& GfxContext, const FRect& Viewrect )
{
	HE_ASSERT( m_pSceneDepthBufferRef != NULL );

	GfxContext.BeginDebugMarker( TEXT( "Geometry Pass" ) );

	// Transition
	GfxContext.TransitionResource( *m_pSceneDepthBufferRef, RS_DepthWrite );
	GfxContext.TransitionResource( GetGBufferColorBuffer( GB_Albedo ), RS_RenderTarget );
	GfxContext.TransitionResource( GetGBufferColorBuffer( GB_Normal ), RS_RenderTarget );
	GfxContext.TransitionResource( GetGBufferColorBuffer( GB_Roughness ), RS_RenderTarget );
	GfxContext.TransitionResource( GetGBufferColorBuffer( GB_Metallic ), RS_RenderTarget );
	GfxContext.TransitionResource( GetGBufferColorBuffer( GB_Specular ), RS_RenderTarget );

	// Set state
	const FColorBuffer* RTs[] = { &m_RenderTargets[0], &m_RenderTargets[1], &m_RenderTargets[2], &m_RenderTargets[3],&m_RenderTargets[4] };
	GfxContext.OMSetRenderTargets(
		GB_NumBuffers,
		RTs,
		m_pSceneDepthBufferRef );
	GfxContext.SetPipelineState( m_PSO );
	GfxContext.SetGraphicsRootSignature( m_RS );
}

void FDeferredShadingTech::GeometryPass::UnBind( FCommandContext& GfxContext )
{
	GfxContext.EndDebugMarker();
}

void FDeferredShadingTech::GeometryPass::ReloadPipeline()
{
	// Create the pipeline state.
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultGeometryPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultGeometryPass.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements = kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = CDepthStencilStateDesc();
	//PSODesc.DepthStencilState.DepthFunc = CF_GreaterEqual;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = GB_NumBuffers;
	for (uint32 i = 0; i < GB_NumBuffers; ++i)
	{
		PSODesc.RTVFormats[i] = m_RenderTargets[i].GetFormat();
	}
	PSODesc.DSVFormat = m_pSceneDepthBufferRef->GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
}

void FDeferredShadingTech::GeometryPass::Resize( const uint32& Width, const uint32& Height ) 
{
	const uint32 kNumMips = 1u;
	m_RenderTargets[GB_Albedo].Create( L"[G-Buffer] Albedo Buffer", Width, Height, kNumMips, kGBufferFormats[GB_Albedo] );
	m_RenderTargets[GB_Normal].Create( L"[G-Buffer] Normal Buffer", Width, Height, kNumMips, kGBufferFormats[GB_Normal] );
	m_RenderTargets[GB_Roughness].Create( L"[G-Buffer] Roughness Buffer", Width, Height, kNumMips, kGBufferFormats[GB_Roughness] );
	m_RenderTargets[GB_Metallic].Create( L"[G-Buffer] Metallic Buffer", Width, Height, kNumMips, kGBufferFormats[GB_Metallic] );
	m_RenderTargets[GB_Specular].Create( L"[G-Buffer] Specular Buffer", Width, Height, kNumMips, kGBufferFormats[GB_Specular] );
}


//
// Light Pass Implementation
//


FDeferredShadingTech::LightPass::LightPass( FDeferredShadingTech::GeometryPass& GeometryPass )
	: m_GeometryPass( GeometryPass )
{

}

FDeferredShadingTech::LightPass::~LightPass()
{

}

void FDeferredShadingTech::LightPass::Initialize( const FVector2& RenderResolution, EFormat SwapchainFormatTEMP )
{
	m_RenderTargetFormat = SwapchainFormatTEMP;

	m_RS.Reset( 9, 1 );
	m_RS.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	m_RS[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RS[kMeshWorld].InitAsConstantBuffer( kMeshWorld, SV_Vertex );
	m_RS[kLights].InitAsConstantBuffer( kLights, SV_Pixel );
	// Pipeline
	// Scene Depth
	m_RS[LRP_GBufferTextureSceneDepth].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureSceneDepth].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	// Albedo
	m_RS[LRP_GBufferTextureAlbedo].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureAlbedo].SetTableRange( 0, DRT_ShaderResourceView, 1, 1 );
	// Normal
	m_RS[LRP_GBufferTextureNormal].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureNormal].SetTableRange( 0, DRT_ShaderResourceView, 2, 1 );
	// Roughness
	m_RS[LRP_GBufferTextureRoughness].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureRoughness].SetTableRange( 0, DRT_ShaderResourceView, 3, 1 );
	// Metallic
	m_RS[LRP_GBufferTextureMetallic].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureMetallic].SetTableRange( 0, DRT_ShaderResourceView, 4, 1 );
	// Specular
	m_RS[LRP_GBufferTextureSpecular].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[LRP_GBufferTextureSpecular].SetTableRange( 0, DRT_ShaderResourceView, 5, 1 );
	m_RS.Finalize( L"[Deferred Renderer][Light Pass] RootSig", RSF_AllowInputAssemblerLayout );


	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/ScreenAlignedQuad.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultLightPass.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.NumElements = kNumScreenSpaceInputElements;
	PSODesc.InputLayout.pInputElementDescs = GScreenSpaceInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState.DepthEnable = false;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = m_RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
}

void FDeferredShadingTech::LightPass::UnInitialize()
{
}

void FDeferredShadingTech::LightPass::Bind( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GfxContext.BeginDebugMarker( TEXT( "Light Pass" ) );

	// Transition
	GfxContext.TransitionResource( *m_pSceneDepthBufferRef, RS_DepthRead );
	GfxContext.TransitionResource( m_GeometryPass.GetGBufferColorBuffer( GB_Albedo ), RS_PixelShaderResource );
	GfxContext.TransitionResource( m_GeometryPass.GetGBufferColorBuffer( GB_Normal ), RS_PixelShaderResource );
	GfxContext.TransitionResource( m_GeometryPass.GetGBufferColorBuffer( GB_Roughness ), RS_PixelShaderResource );
	GfxContext.TransitionResource( m_GeometryPass.GetGBufferColorBuffer( GB_Metallic ), RS_PixelShaderResource );
	GfxContext.TransitionResource( m_GeometryPass.GetGBufferColorBuffer( GB_Specular ), RS_PixelShaderResource );

	// Bind state
	GfxContext.SetPipelineState( m_PSO );
	GfxContext.SetGraphicsRootSignature( m_RS );

	// Bind resources
	GfxContext.SetDepthBufferAsTexture( LRP_GBufferTextureSceneDepth, m_pSceneDepthBufferRef );
	GfxContext.SetColorBufferAsTexture( LRP_GBufferTextureAlbedo, 0, &m_GeometryPass.GetGBufferColorBuffer( GB_Albedo ) );
	GfxContext.SetColorBufferAsTexture( LRP_GBufferTextureNormal, 0, &m_GeometryPass.GetGBufferColorBuffer( GB_Normal ) );
	GfxContext.SetColorBufferAsTexture( LRP_GBufferTextureRoughness, 0, &m_GeometryPass.GetGBufferColorBuffer( GB_Roughness ) );
	GfxContext.SetColorBufferAsTexture( LRP_GBufferTextureMetallic, 0, &m_GeometryPass.GetGBufferColorBuffer( GB_Metallic ) );
	GfxContext.SetColorBufferAsTexture( LRP_GBufferTextureSpecular, 0, &m_GeometryPass.GetGBufferColorBuffer( GB_Specular ) );
}

void FDeferredShadingTech::LightPass::UnBind( FCommandContext& GfxContext )
{
	GfxContext.EndDebugMarker();
}

void FDeferredShadingTech::LightPass::ReloadPipeline()
{
	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultLightPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DefaultLightPass.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.NumElements = kNumScreenSpaceInputElements;
	PSODesc.InputLayout.pInputElementDescs = GScreenSpaceInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState.DepthEnable = false;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = m_RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
}

void FDeferredShadingTech::LightPass::Resize( const uint32& Width, const uint32& Height )
{

}
