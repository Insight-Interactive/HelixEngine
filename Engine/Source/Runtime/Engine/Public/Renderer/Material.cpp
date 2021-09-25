// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/Material.h"
#include "RendererCore.h"
#include "StringHelper.h"
#include "ICommandContext.h"
#include "IConstantBufferManager.h"
#include "Renderer/ConstantBufferStructures.h"
#include "AssetRegistry/AssetDatabase.h"

#include "FileSystem.h"
#include "JsonUtility.h"


void HMaterial::Initialize()
{
	// Init constant buffers
	GConstantBufferManager->CreateConstantBuffer(TEXT("Material Params"), &m_pConstantsCB, sizeof(MaterialConstantsCBData));
}

void HMaterial::UnInitialize()
{
	GConstantBufferManager->DestroyConstantBuffer(m_pConstantsCB->GetUID());
}

void HMaterial::Bind(ICommandContext& GfxContext)
{
	switch (m_Type)
	{
	case MT_Opaque:
		BindOpaque(GfxContext);
		break;
	case MT_Translucent:
		BindTranslucent(GfxContext);
		break;
	default:
		HE_ASSERT(false); // Invalid material type provided when binding for draw.
		break;
	}
}

void HMaterial::LoadFromFile( const String& Filepath )
{
	rapidjson::Document JsonDoc;
	FileRef JsonSource( Filepath.c_str(), FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		const rapidjson::Value& MatRoot = JsonDoc["Material"];
		// Loop over the actor's properties.
		for (uint32 i = 0; i < MatRoot.Size(); ++i)
		{
			const rapidjson::Value& MaterialParam = MatRoot[i];
			
			Char TextureNameBuffer[32];
			
			JsonUtility::GetString( MaterialParam, "AlbedoTexture", TextureNameBuffer, sizeof( TextureNameBuffer ) );
			SetAlbedoTexture( AssetDatabase::GetInstance()->GetTexture( TextureNameBuffer ) );

			JsonUtility::GetString( MaterialParam, "NormalTexture", TextureNameBuffer, sizeof( TextureNameBuffer ) );
			SetNormalTexture( AssetDatabase::GetInstance()->GetTexture( TextureNameBuffer ) );
		}
	}
}

void HMaterial::CreateFromMemory(DataBlob Memory)
{
	Initialize();
	return;
	uint8* Data = Memory.GetBufferPointer();

	// 'A'
	Data++;
	// Albedo Str Len
	uint8 AlbedoStrLen = *Data;
	char* AlbedoStr = (char*)_malloca(AlbedoStrLen + 1); // Allocate the string to store the path + null char
	Data++;

	// N
	Data++;
	uint8 NormalStrLen = *Data;
	char* NormalStr = (char*)_malloca(NormalStrLen + 1); // Allocate the string to store the path + null char
	Data++;

	memcpy(AlbedoStr, Data, AlbedoStrLen);
	AlbedoStr[AlbedoStrLen] = '\0';
	Data += AlbedoStrLen;

	memcpy(NormalStr, Data, NormalStrLen);
	NormalStr[NormalStrLen] = '\0';

	// Load the resources.
	m_AlbedoTexture = GTextureManager->LoadTexture(AlbedoStr, DT_Magenta2D, false);
	m_NormalTexture = GTextureManager->LoadTexture(NormalStr, DT_Magenta2D, false);


	Initialize();
}

void HMaterial::WriteToFile()
{
	// --------
	//  Header
	// --------
	// A # -> Key String and Size in Bytes
	// N # -> Key String and Size in Bytes
	// R # -> Key String and Size in Bytes
	// M # -> Key String and Size in Bytes
	// Albedo tex path
	// Normal tex path
	// Roughness tex path
	// Metallic tex path


	// Set the Length of the textures file path if it is valid.
#define SetTexturePathLength(TexPtr, DstPtr) \
		if (TexPtr.IsValid())\
			*DstPtr = (uint8)TexPtr.GetCacheKey().length();\
		else\
			*DstPtr = 0;\

		// Tex specifier + Int to store path str length + String lengths
	size_t DataLengths = 2 + 2 + m_AlbedoTexture.GetCacheKey().length() + m_NormalTexture.GetCacheKey().length();

	char* pFileMem = (char*)_alloca(DataLengths);
	void* pMemStart = pFileMem;
	ZeroMemory(pFileMem, DataLengths);

	// Write the header.
	*pFileMem = 'A'; pFileMem++;
	SetTexturePathLength(m_AlbedoTexture, pFileMem) pFileMem++;
	*pFileMem = 'N'; pFileMem++;
	SetTexturePathLength(m_NormalTexture, pFileMem) pFileMem++;

	// Wrtie the data.
	String Albedo = m_AlbedoTexture.GetCacheKey();
	memcpy(pFileMem, Albedo.c_str(), Albedo.length());
	pFileMem += Albedo.length();

	String Normal = m_NormalTexture.GetCacheKey();
	memcpy(pFileMem, Normal.c_str(), Normal.length());


	FILE* pFile = NULL;
	fopen_s(&pFile, "M_RustedMetal.ieMat", "wb");
	fwrite(pMemStart, DataLengths, 1, pFile);

	fclose(pFile);

#undef SetTexturePathLength

}

void HMaterial::BindOpaque(ICommandContext& GfxContext)
{
	// Set constants.
	MaterialConstantsCBData* pMat = m_pConstantsCB->GetBufferPointer<MaterialConstantsCBData>();
	pMat->Color = m_ConstantsData.Color;
	GfxContext.SetGraphicsConstantBuffer(kMaterial, m_pConstantsCB);

	// Set Textures.
	{
		GfxContext.SetTexture(GRP_MaterialTextureAlbedo, m_AlbedoTexture);
		GfxContext.SetTexture(GRP_MaterialTextureNormal, m_NormalTexture);
	}
}

void HMaterial::BindTranslucent(ICommandContext& GfxContext)
{
	// TODO Forward transparent pass
}