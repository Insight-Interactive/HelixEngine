// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/Material.h"
#include "RendererCore.h"
#include "CommandContext.h"
#include "ConstantBuffer.h"
#include "Renderer/ConstantBufferStructures.h"
#include "AssetRegistry/AssetDatabase.h"
#include "CommonStructHelpers.h"
#include "Renderer/SceneRenderer.h"


/* static */ MaterialID FMaterial::SNextMaterialID = 0;


FMaterial::FMaterial()
	: m_UID( HE_INVALID_MATERIAL_ID )
	, m_BlendMode( MBM_Opaque )
	, m_Domain( MD_Surface )
	, m_ShadingModel( SM_DefaultLit )
	, m_IsTwoSided( false )
{
	SetMaterialID( SNextMaterialID++ );
	m_ConstantsCB.Create( L"Material Params" );
}

FMaterial::~FMaterial()
{
	m_ConstantsCB.Destroy();
}

void FMaterial::Bind(FCommandContext& GfxContext)
{
	switch (GetShadingModel())
	{
	case SM_DefaultLit:
		BindDefaultLit(GfxContext);
		break;
	case SM_Foliage:
		BindFoliage(GfxContext);
		break;
	default:
		HE_ASSERT(false); // Invalid material type provided when binding for draw.
		break;
	}
}

void FMaterial::LoadFromFile( const String& Filepath )
{
	SetDebugName(StringHelper::GetFilenameFromDirectoryNoExtension(Filepath));

	rapidjson::Document JsonDoc;
	FileRef JsonSource( Filepath.c_str(), FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		enum
		{
			kParameters	= 0,
			kResources	= 1,
			kShaders	= 2,
		};
		const rapidjson::Value& MaterialRoot = JsonDoc["Material"];

		
		// Load the material's parameters.
		const rapidjson::Value& Parameters = MaterialRoot[kParameters];
	
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EMaterialBlendMode ), (int32&)m_BlendMode );
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EMaterialDomain ), (int32&)m_Domain );
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EShadingModel ), (int32&)m_ShadingModel );
		JsonUtility::GetBoolean( Parameters, HE_STRINGIFY( IsTwoSided ), m_IsTwoSided );


		// Load the material's resources.
		const rapidjson::Value& Resources = MaterialRoot[kResources];

		// Load material textures.
		Char TextureGuidBuffer[64];
		ZeroMemory( TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		JsonUtility::GetString( Resources, "AlbedoTexture", TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		FGUID AlbedoGuid = FGUID::CreateFromString( TextureGuidBuffer );
		SetAlbedoTexture( FAssetDatabase::GetInstance()->GetTexture( AlbedoGuid ) );

		ZeroMemory( TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		JsonUtility::GetString( Resources, "NormalTexture", TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		FGUID NormalGuid = FGUID::CreateFromString( TextureGuidBuffer );
		SetNormalTexture( FAssetDatabase::GetInstance()->GetTexture( NormalGuid ) );


		// Load the material's shaders.
		const rapidjson::Value& Shaders = MaterialRoot[kShaders];

		Char ShaderPathBuffer[HE_MAX_PATH];
		ZeroMemory( ShaderPathBuffer, sizeof( ShaderPathBuffer ) );
		JsonUtility::GetString( Shaders, "VertexShader", ShaderPathBuffer, sizeof( ShaderPathBuffer ) );
		m_VertexShaderGuid = FGUID::CreateFromString( ShaderPathBuffer );

		ZeroMemory( ShaderPathBuffer, sizeof( ShaderPathBuffer ) );
		JsonUtility::GetString( Shaders, "PixelShader", ShaderPathBuffer, sizeof( ShaderPathBuffer ) );
		m_PixelShaderGuid = FGUID::CreateFromString( ShaderPathBuffer );

		BuildPipelineState();
	}
}

void FMaterial::BindDefaultLit(FCommandContext& GfxContext) 
{
	// Set constants.
	GfxContext.SetGraphicsRootSignature( m_RootSig );
	GfxContext.SetPipelineState( m_Pipeline );
	GfxContext.SetGraphicsConstantBuffer( kMaterial, m_ConstantsCB );

	// Set Textures.
	{
		if (m_AlbedoTexture.IsValid())
			GfxContext.SetTexture( GRP_MaterialTextureAlbedo, m_AlbedoTexture );

		if (m_NormalTexture.IsValid())
			GfxContext.SetTexture( GRP_MaterialTextureNormal, m_NormalTexture );
	}
}

void FMaterial::BindFoliage(FCommandContext& GfxContext) 
{
	// TODO Forward transparent pass
	// Set constants.
	GfxContext.SetGraphicsRootSignature( m_RootSig );
	GfxContext.SetPipelineState( m_Pipeline );
	GfxContext.SetGraphicsConstantBuffer( kMaterial, m_ConstantsCB );

	// Set Textures.
	{
		if (m_AlbedoTexture.IsValid())
			GfxContext.SetTexture( GRP_MaterialTextureAlbedo, m_AlbedoTexture );

		if (m_NormalTexture.IsValid())
			GfxContext.SetTexture( GRP_MaterialTextureNormal, m_NormalTexture );
	}
}

void FMaterial::BuildPipelineState()
{
	WString RootSigName = L"";
#if HE_DEBUG
	RootSigName += L"Material RootSignature - ";
	RootSigName += StringHelper::UTF8ToUTF16(GetDebugName());
#endif

	m_RootSig.Reset( 6, 1 );
	m_RootSig.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	m_RootSig[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RootSig[kMeshWorld].InitAsConstantBuffer( kMeshWorld, SV_Vertex );
	m_RootSig[kMaterial].InitAsConstantBuffer( kMaterial, SV_Pixel );
	m_RootSig[kLights].InitAsConstantBuffer( kLights, SV_Pixel );
	// Pipeline
	// TODO: These texture slots should be generated dynamically via shader serialization.
	// Albedo
	m_RootSig[GRP_MaterialTextureAlbedo].InitAsDescriptorTable( 1, SV_Pixel );
	m_RootSig[GRP_MaterialTextureAlbedo].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	// Normal
	m_RootSig[GRP_MaterialTextureNormal].InitAsDescriptorTable( 1, SV_Pixel );
	m_RootSig[GRP_MaterialTextureNormal].SetTableRange( 0, DRT_ShaderResourceView, 1, 1 );
	m_RootSig.Finalize(RootSigName.c_str(), RSF_AllowInputAssemblerLayout);

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( FAssetDatabase::GetInstance()->LookupShaderPath( m_VertexShaderGuid ).c_str() );
	DataBlob PSShader = FileSystem::ReadRawData( FAssetDatabase::GetInstance()->LookupShaderPath( m_PixelShaderGuid ).c_str() );

	FPipelineStateDesc PSODesc = { 0 };
	PSODesc.VertexShader					= { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader						= { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs	= GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements			= kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature					= &m_RootSig;
	PSODesc.DepthStencilState				= CDepthStencilStateDesc();
	FBlendDesc BlendDesc					= CBlendDesc();
	switch( GetShadingModel() )
	{
	case SM_Foliage:
		{
			BlendDesc.RenderTarget[0].BlendEnable			= true;
			BlendDesc.RenderTarget[0].SourceBlend			= B_SourceAlpha;
			BlendDesc.RenderTarget[0].DestBlend				= B_InvSourceAlpha;
			BlendDesc.RenderTarget[0].BlendOp				= BO_Add;
			BlendDesc.RenderTarget[0].SourceBlendAlpha		= B_One;
			BlendDesc.RenderTarget[0].DestBlendAlpha		= B_Zero;
			BlendDesc.RenderTarget[0].BlendOpAlpha			= BO_Add;
			BlendDesc.RenderTarget[0].RenderTargetWriteMask = CWE_Red | CWE_Green | CWE_Blue;

			PSODesc.NumRenderTargets = 1;
			PSODesc.RTVFormats[0] = F_R8G8B8A8_UNorm; // TODO: Should be the resolution of the render target.
		}
		break;
	case SM_Unlit:
		{
			PSODesc.NumRenderTargets = 1;
			PSODesc.RTVFormats[0] = F_R8G8B8A8_UNorm; // TODO: Should be the resolution of the render target.
		}
		break;
	case SM_DefaultLit:
		{
			PSODesc.NumRenderTargets = FSceneRenderer::GetNumGBuffers();
			for (uint32 i = 0; i < FSceneRenderer::GetNumGBuffers(); ++i)
			{
				PSODesc.RTVFormats[i] = FSceneRenderer::GetGBufferFormatForBuffer((FDeferredShadingTech::EGBuffers)i);
			}
		}
		break;
	default: 
		HE_ASSERT(false);
		break;
	}
	PSODesc.BlendState = BlendDesc;
	FRasterizerDesc RasterDesc = CRasterizerDesc();
	if (GetIsTwoSided())
	{
		RasterDesc.CullMode = CM_None;
	}
	PSODesc.RasterizerDesc			= RasterDesc;
	PSODesc.SampleMask				= UINT_MAX;
	PSODesc.PrimitiveTopologyType	= PTT_Triangle;
	PSODesc.DSVFormat				= FSceneRenderer::GetSceneDepthBufferForamt();
	PSODesc.SampleDesc				= { 1, 0 };
	m_Pipeline.Initialize( PSODesc );
}