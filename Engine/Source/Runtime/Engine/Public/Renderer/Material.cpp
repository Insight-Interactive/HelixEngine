// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "RendererCore.h"
#include "CommandContext.h"
#include "ConstantBuffer.h"
#include "CommonStructHelpers.h"
#include "Renderer/Material.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/ShaderReflection.h"
#include "Renderer/ConstantBufferStructures.h"
#include "AssetRegistry/AssetDatabase.h"
#include "World/World.h"
#include "Engine/Engine.h"


/* static */ MaterialID FMaterial::SNextMaterialID = 0;


FMaterial::FMaterial()
	: m_UID( HE_INVALID_MATERIAL_ID )
	, m_BlendMode( MBM_Opaque )
	, m_Domain( MD_Surface )
	, m_ShadingModel( SM_DefaultLit )
	, m_IsTwoSided( false )
{
	SetMaterialID( SNextMaterialID++ );
}

FMaterial::~FMaterial()
{
}

void FMaterial::Bind(FCommandContext& GfxContext)
{
	// Set constants.
	GfxContext.SetGraphicsRootSignature(m_RootSig);
	GfxContext.SetPipelineState(m_Pipeline);

	// Bind buffers
	for (auto& Iter : m_ConstBuffersMappings)
	{
		uint32& RootIndex = Iter.second.first;
		std::vector<FConstantBufferInterface*>& ConstBuffers = Iter.second.second;

		if (ConstBuffers.size() > 0)
		{
			FConstantBufferInterface* pBuffer = ConstBuffers[GEngine->GetClientViewport().GetFrameIndex()];
			if (pBuffer)
				GfxContext.SetGraphicsConstantBuffer(RootIndex, *pBuffer);
			else
				HE_ASSERT(false);
		}
	}

	// Bind textures
	for (auto& Iter : m_TextureMappings)
	{
		uint32& RootIndex = Iter.second.first;
		HTextureRef Texture = Iter.second.second;
		
		if(Texture.Get())
			GfxContext.SetTexture(RootIndex, Texture);
		else
			HE_ASSERT(false);
	}
}

void FMaterial::Destroy()
{
	m_ConstBuffersMappings.clear();
	m_TextureMappings.clear();
	for (auto& Iter : m_ConstBufferVarMappings)
	{
		for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
		{
			if (Iter.second[i].first)
			{
				delete Iter.second[i].first; // Failed because multiple entries in m_ConstBufferVarMappings can reference the same const buffer memory location.
				Iter.second[i].first = nullptr;
			}
		}
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
			kParameters				= 0,
			kTextureResources		= 1,
			kShaderConstVariables	= 2,
			kShaders				= 3,
		};
		const rapidjson::Value& MaterialRoot = JsonDoc["Material"];

		
		// Load the material's parameters.
		const rapidjson::Value& Parameters = MaterialRoot[kParameters];
	
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EMaterialBlendMode ), (int32&)m_BlendMode );
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EMaterialDomain ), (int32&)m_Domain );
		JsonUtility::GetInteger( Parameters, HE_STRINGIFY( EShadingModel ), (int32&)m_ShadingModel );
		JsonUtility::GetBoolean( Parameters, HE_STRINGIFY( IsTwoSided ), m_IsTwoSided );


		// Load the material's resources.
		const rapidjson::Value& Resources = MaterialRoot[kTextureResources];
		const rapidjson::Value& ShaderConstVariables = MaterialRoot[kShaderConstVariables];
		
		// Load material textures.
		//Char TextureGuidBuffer[64];
		//ZeroMemory( TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		//JsonUtility::GetString( Resources, "AlbedoTexture", TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		//FGUID AlbedoGuid = FGUID::CreateFromString( TextureGuidBuffer );
		//SetAlbedoTexture( FAssetDatabase::GetInstance()->GetTexture( AlbedoGuid ) );

		//ZeroMemory( TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		//JsonUtility::GetString( Resources, "NormalTexture", TextureGuidBuffer, sizeof( TextureGuidBuffer ) );
		//FGUID NormalGuid = FGUID::CreateFromString( TextureGuidBuffer );
		//SetNormalTexture( FAssetDatabase::GetInstance()->GetTexture( NormalGuid ) );


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

		// Set textures.
		for (rapidjson::Value::ConstMemberIterator itr = Resources.MemberBegin();
			itr != Resources.MemberEnd(); ++itr)
		{
			const Char* ResourceName = itr->name.GetString();
			const Char* GuidStr = itr->value.GetString();
			FGUID TextureGuid = FGUID::CreateFromString(GuidStr);

			SetTexture(ResourceName, FAssetDatabase::GetInstance()->GetTexture(TextureGuid));
		}
		
		// Set constant variables.
		for (rapidjson::Value::ConstMemberIterator itr = ShaderConstVariables.MemberBegin();
			itr != ShaderConstVariables.MemberEnd(); ++itr)
		{
			const Char* VarName = itr->name.GetString();
			float VarValue = itr->value.GetFloat();
			SetFloat(VarName, VarValue);
		}

	}

}

void FMaterial::BuildPipelineState()
{
	WString RootSigName = L"";
#if HE_DEBUG
	RootSigName += L"Material RootSignature - ";
	RootSigName += StringHelper::UTF8ToUTF16(GetDebugName());
#endif

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( FAssetDatabase::GetInstance()->LookupShaderPath( m_VertexShaderGuid ).c_str() );
	DataBlob PSShader = FileSystem::ReadRawData( FAssetDatabase::GetInstance()->LookupShaderPath( m_PixelShaderGuid ).c_str() );

	FShaderReflection VertexReflection(VSShader.GetBufferPointer(), VSShader.GetDataSize());
	FShaderReflection PixelReflection(PSShader.GetBufferPointer(), PSShader.GetDataSize());

	const uint32 kNumStaticSamplers = 1;
	const uint32 NumResources = (PixelReflection.GetNumBoundResources() + VertexReflection.GetNumBoundResources()) - kNumStaticSamplers;
	m_RootSig.Reset(NumResources, kNumStaticSamplers);

	ReflectShader(m_RootSig, SV_Vertex, VertexReflection);
	ReflectShader(m_RootSig, SV_Pixel,  PixelReflection);
	m_RootSig.Finalize(RootSigName.c_str(), RSF_AllowInputAssemblerLayout);


	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader					= { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader						= { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs	= GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements			= kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature					= &m_RootSig;
	PSODesc.DepthStencilState				= CDepthStencilStateDesc();
	FBlendDesc BlendDesc					= CBlendDesc();
	switch( GetShadingModel() )
	{
	case SM_DefaultLit:
	{
		const uint32 NumGBuffers = FSceneRenderer::GetNumGBuffers();
		PSODesc.NumRenderTargets = NumGBuffers;
		for (uint32 i = 0; i < NumGBuffers; ++i)
		{
			PSODesc.RTVFormats[i] = FSceneRenderer::GetGBufferFormatForBuffer((FDeferredShadingTech::EGBuffers)i);
		}
	}
	break;
	case SM_Unlit:
	case SM_Foliage:
	{
		// Only unlit and transparent are rendered in the forward pass.

		BlendDesc.RenderTarget[0].BlendEnable			= true;
		BlendDesc.RenderTarget[0].SourceBlend			= B_SourceAlpha;
		BlendDesc.RenderTarget[0].DestBlend				= B_InvSourceAlpha;
		BlendDesc.RenderTarget[0].BlendOp				= BO_Add;
		BlendDesc.RenderTarget[0].SourceBlendAlpha		= B_One;
		BlendDesc.RenderTarget[0].DestBlendAlpha		= B_Zero;
		BlendDesc.RenderTarget[0].BlendOpAlpha			= BO_Add;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = CWE_ColorOnly;

		PSODesc.NumRenderTargets = 1;
		PSODesc.RTVFormats[0] = F_R8G8B8A8_UNorm; // TODO: Should be the resolution of the render target.
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
		RasterDesc.CullMode	= CM_None;
	}
	PSODesc.RasterizerDesc			= RasterDesc;
	PSODesc.SampleMask				= UINT_MAX;
	PSODesc.PrimitiveTopologyType	= PTT_Triangle;
	PSODesc.DSVFormat				= FSceneRenderer::GetSceneDepthBufferForamt();
	PSODesc.SampleDesc				= { 1, 0 };
	m_Pipeline.Initialize( PSODesc );
}

void FMaterial::ReflectShader(FRootSignature& outSignature, EShaderVisibility ShaderType, const FShaderReflection& Reflector)
{
	HE_ASSERT(Reflector.GetNumBoundResources() != kInvalidShaderResourceCount);

	FSceneRenderer& SceneRenderer = GEngine->GetClientViewport().GetSceneRenderer(); // TODO: This should the viewport of whatever the material is being rendered onto.

	for (uint32 i = 0; i < Reflector.GetNumBoundResources(); i++)
	{
		FShaderResourceDescription ResourceDesc = { };
		Reflector.GetResourceBindingDescripion(i, ResourceDesc);
		if (ResourceDesc.Name == "")
		{
			HE_LOG(Warning, TEXT("Shader resource has empty name! This does not make sense and is not allowed."));
			HE_ASSERT(false);
		}

		const uint32 Register = ResourceDesc.Register;
		const StringHashValue NameHash = StringHash(ResourceDesc.Name);
		const uint32 NextAvailableRootIndex = outSignature.GetNextUnoccupiedIndex();
		HE_ASSERT(NextAvailableRootIndex != -1); // There are no more available indicies in root signature!

		const bool IsBufferMapped	= m_ConstBuffersMappings.find(NameHash) != m_ConstBuffersMappings.end();
		const bool IsTextureMapped	= m_TextureMappings.find(NameHash) != m_TextureMappings.end();
		if (IsBufferMapped || IsTextureMapped)
			continue; // The root parameter at this index has already been initialized, just move to the next available one.


		switch (ResourceDesc.Type)
		{
		case RT_ConstantBuffer:
		{
			FConstantBufferReflection ConstBuffer;
			Reflector.GetConstantBufferByName(ResourceDesc.Name, ConstBuffer);

			outSignature[NextAvailableRootIndex].InitAsConstantBuffer(Register, ShaderType);
			bool ReservedBufferExists = SceneRenderer.ReservedBufferExistsByHashName(NameHash);
			if (ReservedBufferExists)
			{
				if (Register == kSceneConstants) // Allow all shaders to access the common constant variables.
					outSignature[NextAvailableRootIndex].SetVisibility(SV_All);

				// Create the element, but don't add the buffer pointers's. We have to add them for each frame.
				m_ConstBuffersMappings.emplace(NameHash, std::make_pair(NextAvailableRootIndex, NULL));
				auto& PerFrameBuffers = m_ConstBuffersMappings.at(NameHash).second;

				// Reserve space for each buffer per-frame.
				PerFrameBuffers.resize(HE_MAX_SWAPCHAIN_BACK_BUFFERS);

				for (uint32 j = 0; j < HE_MAX_SWAPCHAIN_BACK_BUFFERS; j++)
				{
					// Bind a pointer to the pre allocated buffer.
					PerFrameBuffers[j] = SceneRenderer.GetReservedConstantBufferByHashNameForFrame(NameHash, j);
				}
			}
			else
			{
				// There is not a pre-existing buffer, the user might have declared a new buffer in the shader so create it.

				// Create the element, but don't add the buffer pointers's. We have to add them for each frame later*.
				auto& Result = m_ConstBuffersMappings.emplace(NameHash, std::make_pair(NextAvailableRootIndex, NULL));

				if (Register != kMeshWorld) // Each mesh will have its own constant buffer, so, we dont have to have the material create and bind one for us.
				{
					auto& Iter = m_ConstBuffersMappings.at(NameHash);
					auto& PerFrameBuffers = Iter.second;
					PerFrameBuffers = std::vector<FConstantBufferInterface*>{};

					for (uint32 j = 0; j < HE_MAX_SWAPCHAIN_BACK_BUFFERS; j++)
					{
						// Create and bind a pointer to the newly allocated buffer.
						PerFrameBuffers.emplace_back( new FConstantBuffer() );

						// Initialize the buffer.
						WString DebugName = L"";
#				if HE_DEBUG
						DebugName = StringHelper::UTF8ToUTF16(ResourceDesc.Name);
#				endif
						PerFrameBuffers[j]->Create(DebugName.c_str(), ConstBuffer.SizeInBytes);

						// Make the links to the variables in the shader const buffers.
						{
							FConstantBuffer* pConstBuffer = SCast<FConstantBuffer*>(PerFrameBuffers[j]);
							HE_ASSERT(pConstBuffer != NULL);
							
							uint8* pBufferPtr = pConstBuffer->GetBufferPointer();
							for (uint32 k = 0; k < ConstBuffer.Variables.size(); k++)
							{
								FShaderVariableDescription& Variable = ConstBuffer.Variables[k];
								
								StringHashValue VarHash = StringHash(Variable.Name);

								// If the vector wasn't initialized, initialize it.
								if (m_ConstBufferVarMappings[VarHash].size() == 0)
								{
									m_ConstBufferVarMappings[VarHash] = std::vector< std::pair<FConstantBufferInterface*, uint8*> >{};
									m_ConstBufferVarMappings[VarHash].resize( HE_MAX_SWAPCHAIN_BACK_BUFFERS );
								}
								
								// Map the pointer for this frame only.
								m_ConstBufferVarMappings.at(VarHash)[j] = std::make_pair(PerFrameBuffers[j], pBufferPtr );

								// Move to the next variable in the shader's constant buffer.
								pBufferPtr += Variable.SizeInBtytes;
							}
						}
					}
				}
			}
			break;
		}
		case RT_Texture:
		{
			// Create the texture but nullify it, the user will decide which textures get set.
			// TODO: When binding textures from m_TextureMappings, check to see if the texture is not null first!
			m_TextureMappings.emplace(NameHash, std::make_pair(NextAvailableRootIndex, nullptr));

			outSignature[NextAvailableRootIndex].InitAsDescriptorTable(1, ShaderType);
			outSignature[NextAvailableRootIndex].SetTableRange(0, DRT_ShaderResourceView, Register, 1);

			break;
		}
		case RT_Sampler:
		{
			outSignature.InitStaticSampler(Register, GLinearWrapSamplerDesc, ShaderType);

			break;
		}
		case RT_Undefined:
		default:
			HE_ASSERT(false);
			break;
		}

	}
}
