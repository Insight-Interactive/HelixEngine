// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/AssetDatabase.h"

#include "Engine/Engine.h"
#include "Engine/GameProject.h"


/*static*/ FAssetDatabase* FAssetDatabase::SInstance = nullptr;

FAssetDatabase::FAssetDatabase()
{
	if (SInstance == nullptr)
	{
		SInstance = this;
	}
	else
	{
		HE_ASSERT( false );
		HE_LOG( Error, TEXT( "Asset database already exists!" ) );
	}
}

FAssetDatabase::~FAssetDatabase()
{
}

/*static*/ void FAssetDatabase::Initialize()
{
	// Load all the asset databases.
	rapidjson::Document JsonDoc;

	char ManifestPath[HE_MAX_PATH];
	FGameProject::GetInstance()->GetProjectDirectoryFullPath( kAssetManifestFilename, ManifestPath, sizeof( ManifestPath ) );
	FileRef JsonSource( ManifestPath , FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		GFontManager.Initialize();
		
		enum
		{
			// AssetManifest.json JSON element indicies

			kModelsDbIndex		= 0,
			kTexturesDbIndex	= 1,
			kMaterialsDbIndex	= 2,
			kActorsDbIndex		= 3,
			kShadersDbIndex		= 4,
			kScriptDbIndex		= 5,
			kFontDBIndex		= 6
		};

		const rapidjson::Value& AssetDbRoot = JsonDoc[HE_STRINGIFY( FAssetDatabase )];

		SInstance->m_MeshDatabase.Deserialize( AssetDbRoot[kModelsDbIndex] );
		SInstance->m_TextureDatabase.Deserialize( AssetDbRoot[kTexturesDbIndex] );
		SInstance->m_MaterialDatabase.Deserialize( AssetDbRoot[kMaterialsDbIndex] );
		SInstance->m_ActorDatabase.Deserialize( AssetDbRoot[kActorsDbIndex] );
		SInstance->m_ShaderDatabase.Deserialize( AssetDbRoot[kShadersDbIndex] );
		SInstance->m_ScriptDatabase.Deserialize( AssetDbRoot[kScriptDbIndex] );
		SInstance->m_FontDatabase.Deserialize( AssetDbRoot[kFontDBIndex] );

		SInstance->m_MeshDatabase.Initialize();
		SInstance->m_TextureDatabase.Initialize();
		SInstance->m_MaterialDatabase.Initialize();
		SInstance->m_ActorDatabase.Initialize();
		SInstance->m_ShaderDatabase.Initialize();
		SInstance->m_ScriptDatabase.Initialize();
		SInstance->m_FontDatabase.Initialize();
	}
	else
	{
		System::CreateMessageBox( L"No asset manifest found!", L"Missing data", System::MDI_Ok, System::MDIcon_Critical );
		GEngine->RequestShutdown();
	}
}

/*static*/ void FAssetDatabase::Uninitialize()
{
	HE_LOG( Log, TEXT( "Clearing asset databases." ) );

	SInstance->m_MeshDatabase.UnInitialize();
	SInstance->m_TextureDatabase.UnInitialize();
	SInstance->m_MaterialDatabase.UnInitialize();
	SInstance->m_ActorDatabase.UnInitialize();
	SInstance->m_ShaderDatabase.UnInitialize();
	SInstance->m_ScriptDatabase.UnInitialize();
	
	GFontManager.UnInitialize();
}
