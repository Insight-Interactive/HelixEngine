#include "EnginePCH.h"

#include "AssetRegistry/AssetDatabase.h"


FAssetDatabase::FAssetDatabase()
{
}

FAssetDatabase::~FAssetDatabase()
{
}

void FAssetDatabase::Initialize( const Char* ManifestFile )
{
	// Load all the asset databases.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( ManifestFile, FUM_Read, CM_Text );
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

		m_MeshDatabase.Deserialize( AssetDbRoot[kModelsDbIndex] );
		m_TextureDatabase.Deserialize( AssetDbRoot[kTexturesDbIndex] );
		m_MaterialDatabase.Deserialize( AssetDbRoot[kMaterialsDbIndex] );
		m_ActorDatabase.Deserialize( AssetDbRoot[kActorsDbIndex] );
		m_ShaderDatabase.Deserialize( AssetDbRoot[kShadersDbIndex] );

		m_MeshDatabase.Initialize();
		m_TextureDatabase.Initialize();
		m_MaterialDatabase.Initialize();
		m_ActorDatabase.Initialize();
		m_ShaderDatabase.Initialize();
	}
}

void FAssetDatabase::Uninitialize()
{
	HE_LOG( Log, TEXT( "Clearing asset databases." ) );

	m_MeshDatabase.UnInitialize();
	m_TextureDatabase.UnInitialize();
	m_MaterialDatabase.UnInitialize();
	m_ActorDatabase.UnInitialize();
}
