#include "EnginePCH.h"

#include "AssetRegistry/AssetDatabase.h"

#include "FileSystem.h"
#include "JsonUtility.h"
#include "StringHelper.h"


AssetDatabase::AssetDatabase()
{
}

AssetDatabase::~AssetDatabase()
{
}

void AssetDatabase::Initialize( const Char* DatabaseFile )
{
	// Load all the asset databases.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( DatabaseFile, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		const rapidjson::Value& AssetDbRoot = JsonDoc["AssetDatabase"];
		// Loop over the actor's properties.
		for (uint32 i = 0; i < AssetDbRoot.Size(); ++i)
		{
			const rapidjson::Value& DbParam = AssetDbRoot[i];

			// Database Name
			Char StringBuff[HE_MAX_PATH];

			JsonUtility::GetString( DbParam, "MeshDatabase", StringBuff, sizeof( StringBuff ) );
			m_MeshDatabase.Initialize( StringBuff );

			JsonUtility::GetString( DbParam, "TextureDatabase", StringBuff, sizeof( StringBuff ) );
			m_TextureDatabase.Initialize( StringBuff );

			JsonUtility::GetString( DbParam, "MaterialDatabase", StringBuff, sizeof( StringBuff ) );
			m_MaterialDatabase.Initialize( StringBuff );

			JsonUtility::GetString( DbParam, "ActorDatabase", StringBuff, sizeof( StringBuff ) );
			m_ActorDatabase.Initialize( StringBuff );
		}
	}
}

void AssetDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing asset databases." ) );

	m_MeshDatabase.UnInitialize();
	m_TextureDatabase.UnInitialize();
	m_MaterialDatabase.UnInitialize();
	m_ActorDatabase.UnInitialize();
}
