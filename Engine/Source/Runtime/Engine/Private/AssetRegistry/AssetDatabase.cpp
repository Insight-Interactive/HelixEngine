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

			kActorsDbIndex		= 0,
			kShadersDbIndex		= 1,
			kFontDBIndex		= 2
		};

		const rapidjson::Value& AssetDbRoot = JsonDoc[HE_STRINGIFY( FAssetDatabase )];

		SInstance->m_ActorDatabase.Deserialize( AssetDbRoot[kActorsDbIndex] );
		SInstance->m_ShaderDatabase.Deserialize( AssetDbRoot[kShadersDbIndex] );
		SInstance->m_FontDatabase.Deserialize( AssetDbRoot[kFontDBIndex] );

		SInstance->m_ActorDatabase.Initialize();
		SInstance->m_ShaderDatabase.Initialize();
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

	SInstance->m_ActorDatabase.UnInitialize();
	SInstance->m_ShaderDatabase.UnInitialize();
	
	GFontManager.UnInitialize();
}
