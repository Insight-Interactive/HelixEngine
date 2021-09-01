// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/Technique/DeferredShadingTech.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/Common.h"
#include "CommonStructHelpers.h"
#include "DataTypeWrappers.h"
#include "FileSystem.h"
#include "RendererCore.h"
#include "IDevice.h"
#include "IGpuResource.h"
#include "IRootSignature.h"
#include "ICommandContext.h"
#include "IDepthBuffer.h"
#include "IPipelineState.h"
#include "IColorBuffer.h"



//
// Deferred Shading Tech Implementation
//

DeferredShadingTech::DeferredShadingTech()
	: m_LightPass(m_GeometryPass)
{

}

DeferredShadingTech::~DeferredShadingTech()
{

}

void DeferredShadingTech::Initialize(FVector2 RenderResolution, EFormat SwapchainFormat)
{
	m_GeometryPass.Initialize(RenderResolution);
	m_LightPass.Initialize(RenderResolution, SwapchainFormat);
}

void DeferredShadingTech::UnInitialize()
{
	m_GeometryPass.UnInitialize();
	m_LightPass.UnInitialize();
}

//
// Geometry Pass Implementation
//

DeferredShadingTech::GeometryPass::GeometryPass()
{

}

DeferredShadingTech::GeometryPass::~GeometryPass()
{

}

void DeferredShadingTech::GeometryPass::Initialize(const FVector2& RenderResolution)
{
	GDevice->CreateRootSignature(&m_pRS);
	m_pRS->Reset(6, 1);
	(*m_pRS).InitStaticSampler(0, GLinearWrapSamplerDesc, SV_Pixel);
	// Common
	(*m_pRS)[kSceneConstants].InitAsConstantBuffer(kSceneConstants, SV_All);
	(*m_pRS)[kMeshWorld].InitAsConstantBuffer(kMeshWorld, SV_Vertex);
	(*m_pRS)[kMaterial].InitAsConstantBuffer(kMaterial, SV_Pixel);
	(*m_pRS)[kLights].InitAsConstantBuffer(kLights, SV_Pixel);
	// Pipeline
	// Albedo
	(*m_pRS)[GRP_MaterialTextureAlbedo].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[GRP_MaterialTextureAlbedo].SetTableRange(0, DRT_ShaderResourceView, 0, 1);
	// Normal
	(*m_pRS)[GRP_MaterialTextureNormal].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[GRP_MaterialTextureNormal].SetTableRange(0, DRT_ShaderResourceView, 1, 1);
	(*m_pRS).Finalize(L"[Deferred Renderer][Geometry Pass] RootSig", RSF_AllowInputAssemblerLayout);


	// Create the render targets.
	//
	const uint32 NumMips = 1u;
	GDevice->CreateColorBuffer(TEXT("[Geometry Pass][G-Buffer] Albedo Buffer"), (uint32)RenderResolution.x, (uint32)RenderResolution.y, NumMips, F_R8G8B8A8_UNorm, &m_RenderTargets[GB_Albedo]);
	GDevice->CreateColorBuffer(TEXT("[Geometry Pass][G-Buffer] Normal Buffer"), (uint32)RenderResolution.x, (uint32)RenderResolution.y, NumMips, F_R8G8B8A8_UNorm, &m_RenderTargets[GB_Normal]);
	GDevice->CreateColorBuffer(TEXT("[Geometry Pass][G-Buffer] Position Buffer"), (uint32)RenderResolution.x, (uint32)RenderResolution.y, NumMips, F_R32G32B32A32_Float, &m_RenderTargets[GB_Position]);


	// Create the depth buffer.
	//
	GDevice->CreateDepthBuffer(TEXT("[Geometry Pass] Scene Depth Buffer"), (uint32)RenderResolution.x, (uint32)RenderResolution.y, F_D32_Float, &m_pDepthBuffer);


	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData("Shaders/GenericGeometryPass.vs.cso");
	DataBlob PSShader = FileSystem::ReadRawData("Shaders/GenericGeometryPass.ps.cso");


	PipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements = kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature = m_pRS;
	PSODesc.DepthStencilState = CDepthStencilStateDesc();
	//PSODesc.DepthStencilState.DepthFunc = CF_GreaterEqual;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = GB_NumBuffers;
	for (uint32 i = 0; i < GB_NumBuffers; ++i)
	{
		PSODesc.RTVFormats[i] = DCast<IPixelBuffer*>(m_RenderTargets[i])->GetFormat();
	}
	PSODesc.DSVFormat = DCast<IPixelBuffer*>(m_pDepthBuffer)->GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	GDevice->CreatePipelineState(PSODesc, &m_pPSO);


	m_pDepthBufferGPUResource = DCast<IGpuResource*>(m_pDepthBuffer);;
}

void DeferredShadingTech::GeometryPass::UnInitialize()
{
	HE_SAFE_DELETE_PTR(m_pDepthBuffer);
	HE_SAFE_DELETE_PTR(m_pPSO);
	HE_SAFE_DELETE_PTR(m_pRS);
	for (uint32 i = 0; i < GB_NumBuffers; ++i)
	{
		HE_SAFE_DELETE_PTR(m_RenderTargets[i]);
	}
}

void DeferredShadingTech::GeometryPass::Bind( ICommandContext& GfxContext, const Rect& Viewrect )
{
	GfxContext.BeginDebugMarker(TEXT("Geometry Pass"));

	// Transition
	// 
	// Color
	IGpuResource& AlbedoGBufferResource = *GetGBufferColorBuffer( GB_Albedo ).As<IGpuResource*>();
	GfxContext.TransitionResource(AlbedoGBufferResource, RS_RenderTarget);
	IGpuResource& NormalGBufferResource = *GetGBufferColorBuffer( GB_Normal ).As<IGpuResource*>();
	GfxContext.TransitionResource(NormalGBufferResource, RS_RenderTarget);
	IGpuResource& PositionGBufferResource = *GetGBufferColorBuffer( GB_Position ).As<IGpuResource*>();
	GfxContext.TransitionResource(PositionGBufferResource, RS_RenderTarget);
	// Depth
	IGpuResource& DepthBufferResource = *GetSceneDepthBuffer()->As<IGpuResource*>();
	GfxContext.TransitionResource(DepthBufferResource, RS_DepthWrite);

	// Clear
	//
	GfxContext.ClearColorBuffer(GetGBufferColorBuffer(GB_Albedo), Viewrect);
	GfxContext.ClearColorBuffer(GetGBufferColorBuffer(GB_Normal), Viewrect);
	GfxContext.ClearColorBuffer(GetGBufferColorBuffer(GB_Position), Viewrect);
	GfxContext.ClearDepth(*GetSceneDepthBuffer());


	// Set
	//
	const IColorBuffer* RTs[] =
	{
		m_RenderTargets[0], m_RenderTargets[1], m_RenderTargets[2]
	};
	GfxContext.OMSetRenderTargets(
		GB_NumBuffers,
		RTs,
		GetSceneDepthBuffer());
	GfxContext.SetPipelineState(*m_pPSO);
	GfxContext.SetGraphicsRootSignature(*m_pRS);
}

void DeferredShadingTech::GeometryPass::UnBind(ICommandContext& GfxContext)
{
	IGpuResource& AlbedoGBufferResource = *GetGBufferColorBuffer( GB_Albedo ).As<IGpuResource*>();
	GfxContext.TransitionResource(AlbedoGBufferResource, RS_PixelShaderResource);

	IGpuResource& NormalGBufferResource = *GetGBufferColorBuffer( GB_Normal ).As<IGpuResource*>();
	GfxContext.TransitionResource(NormalGBufferResource, RS_PixelShaderResource);

	IGpuResource& PositionGBufferResource = *GetGBufferColorBuffer( GB_Position ).As<IGpuResource*>();
	GfxContext.TransitionResource(PositionGBufferResource, RS_PixelShaderResource);

	IGpuResource& DepthBuffferResource = *GetSceneDepthBuffer()->As<IGpuResource*>();
	GfxContext.TransitionResource(DepthBuffferResource, RS_DepthRead);

	GfxContext.EndDebugMarker();
}

EFormat DeferredShadingTech::GeometryPass::GetDepthFormat() const
{
	return DCast<IPixelBuffer*>(m_pDepthBuffer)->GetFormat();
}


//
// Light Pass Implementation
//


DeferredShadingTech::LightPass::LightPass( DeferredShadingTech::GeometryPass& GeometryPass )
	: m_GeometryPass( GeometryPass )
{

}

DeferredShadingTech::LightPass::~LightPass()
{

}

void DeferredShadingTech::LightPass::Initialize(const FVector2& RenderResolution, EFormat SwapchainFormatTEMP)
{
	GDevice->CreateRootSignature(&m_pRS);
	m_pRS->Reset(8, 1);
	(*m_pRS).InitStaticSampler(0, GLinearWrapSamplerDesc, SV_Pixel);
	// Common
	(*m_pRS)[kSceneConstants].InitAsConstantBuffer(kSceneConstants, SV_All);
	(*m_pRS)[kMeshWorld].InitAsConstantBuffer(kMeshWorld, SV_Vertex);
	(*m_pRS)[kMaterial].InitAsConstantBuffer(kMaterial, SV_Pixel);
	(*m_pRS)[kLights].InitAsConstantBuffer(kLights, SV_Pixel);
	// Pipeline
	// Scene Depth
	(*m_pRS)[LRP_GBufferTextureSceneDepth].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[LRP_GBufferTextureSceneDepth].SetTableRange(0, DRT_ShaderResourceView, 0, 1);
	// Albedo
	(*m_pRS)[LRP_GBufferTextureAlbedo].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[LRP_GBufferTextureAlbedo].SetTableRange(0, DRT_ShaderResourceView, 1, 1);
	// Normal
	(*m_pRS)[LRP_GBufferTextureNormal].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[LRP_GBufferTextureNormal].SetTableRange(0, DRT_ShaderResourceView, 2, 1);
	// Position
	(*m_pRS)[LRP_GBufferTexturePosition].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[LRP_GBufferTexturePosition].SetTableRange(0, DRT_ShaderResourceView, 3, 1);
	(*m_pRS).Finalize(L"[Deferred Renderer][Light Pass] RootSig", RSF_AllowInputAssemblerLayout);


	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData("Shaders/GenericLightPass.vs.cso");
	DataBlob PSShader = FileSystem::ReadRawData("Shaders/GenericLightPass.ps.cso");

	PipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.NumElements = kNumScreenSpaceInputElements;
	PSODesc.InputLayout.pInputElementDescs = GScreenSpaceInputElements;
	PSODesc.pRootSignature = m_pRS;
	PSODesc.DepthStencilState.DepthEnable = false;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = SwapchainFormatTEMP;// GetPassResultFormat();
	PSODesc.SampleDesc = { 1, 0 };
	GDevice->CreatePipelineState(PSODesc, &m_pPSO);
}

void DeferredShadingTech::LightPass::UnInitialize()
{
	HE_SAFE_DELETE_PTR(m_pRS);
	HE_SAFE_DELETE_PTR(m_pPSO);
}

void DeferredShadingTech::LightPass::Bind(ICommandContext& GfxContext, const Rect& Viewrect)
{
	GfxContext.BeginDebugMarker(TEXT("Light Pass"));

	GfxContext.SetPipelineState(*m_pPSO);
	GfxContext.SetGraphicsRootSignature(*m_pRS);

	GfxContext.SetDepthBufferAsTexture(LRP_GBufferTextureSceneDepth,   m_GeometryPass.GetSceneDepthBuffer());
	GfxContext.SetColorBufferAsTexture(LRP_GBufferTextureAlbedo,   0, &m_GeometryPass.GetGBufferColorBuffer(GB_Albedo));
	GfxContext.SetColorBufferAsTexture(LRP_GBufferTextureNormal,   0, &m_GeometryPass.GetGBufferColorBuffer(GB_Normal));
	GfxContext.SetColorBufferAsTexture(LRP_GBufferTexturePosition, 0, &m_GeometryPass.GetGBufferColorBuffer(GB_Position));
}

void DeferredShadingTech::LightPass::UnBind(ICommandContext& GfxContext)
{
	GfxContext.EndDebugMarker();
}
