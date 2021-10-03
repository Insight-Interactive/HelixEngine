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
			String ProjectRelativePath;

			JsonUtility::GetString( DbParam, "MeshDatabase", StringBuff, sizeof( StringBuff ) );
			ProjectRelativePath = FGameProject::GetInstance()->GetProjectRoot() + StringBuff;
			m_MeshDatabase.Initialize( ProjectRelativePath.c_str() );

			JsonUtility::GetString( DbParam, "TextureDatabase", StringBuff, sizeof( StringBuff ) );
			ProjectRelativePath = FGameProject::GetInstance()->GetProjectRoot() + StringBuff;
			m_TextureDatabase.Initialize( ProjectRelativePath.c_str() );

			JsonUtility::GetString( DbParam, "MaterialDatabase", StringBuff, sizeof( StringBuff ) );
			ProjectRelativePath = FGameProject::GetInstance()->GetProjectRoot() + StringBuff;
			m_MaterialDatabase.Initialize( ProjectRelativePath.c_str() );

			JsonUtility::GetString( DbParam, "ActorDatabase", StringBuff, sizeof( StringBuff ) );
			ProjectRelativePath = FGameProject::GetInstance()->GetProjectRoot() + StringBuff;
			m_ActorDatabase.Initialize( ProjectRelativePath.c_str() );
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
