// Copyright 2021 Insight Interactive. All Rights Reserved.
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
	FileRef JsonSource( FGameProject::GetInstance()->GetProjectRoot() + kAssetManifestFilename, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		enum
		{
			kModelsDbIndex		= 0,
			kTexturesDbIndex	= 1,
			kMaterialsDbIndex	= 2,
			kActorsDbIndex		= 3,
			kShadersDbIndex		= 4
		};

		const rapidjson::Value& AssetDbRoot = JsonDoc[HE_STRINGIFY( FAssetDatabase )];

		SInstance->m_MeshDatabase.Deserialize( AssetDbRoot[kModelsDbIndex] );
		SInstance->m_TextureDatabase.Deserialize( AssetDbRoot[kTexturesDbIndex] );
		SInstance->m_MaterialDatabase.Deserialize( AssetDbRoot[kMaterialsDbIndex] );
		SInstance->m_ActorDatabase.Deserialize( AssetDbRoot[kActorsDbIndex] );
		SInstance->m_ShaderDatabase.Deserialize( AssetDbRoot[kShadersDbIndex] );

		SInstance->m_MeshDatabase.Initialize();
		SInstance->m_TextureDatabase.Initialize();
		SInstance->m_MaterialDatabase.Initialize();
		SInstance->m_ActorDatabase.Initialize();
		SInstance->m_ShaderDatabase.Initialize();
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
}
