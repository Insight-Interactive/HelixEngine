#include "EnginePCH.h"

#include "AssetRegistry/MaterialDatabase.h"
#include "FileSystem.h"
#include "JsonUtility.h"


MaterialDatabase::MaterialDatabase()
	: DatabaseInterface( "Material Database" )
{
}

MaterialDatabase::~MaterialDatabase()
{
}

void MaterialDatabase::Initialize( const Char* MaterialDatabaseFile )
{
	// Load the texture cache.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( MaterialDatabaseFile, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator itr = JsonDoc.MemberBegin();
			itr != JsonDoc.MemberEnd(); ++itr)
		{
			String MatName = itr->name.GetString();
			String MatPath = itr->value.GetString();
			HE_LOG( Log, TEXT( "Loading material into database: %s with path %s" ), CharToTChar( MatName ), CharToTChar( MatPath ) );
			m_Data[MatName] = MatPath;
		}
	}
}

void MaterialDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing material database." ) );
	m_Data.clear();
}

void MaterialDatabase::SerializeToFile_Implementation( const Char* Filepath )
{
	rapidjson::StringBuffer StrBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer( StrBuffer );

	Writer.StartObject();
	{
		for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
		{
			Writer.Key( Iter->first.c_str() );
			Writer.String( Iter->second.c_str() );
		}
	}
	Writer.EndObject();

	FileRef OutFile( Filepath, FUM_Write, CM_Text );
	if (OutFile->IsOpen())
	{
		if (!OutFile->WriteData( (void*)StrBuffer.GetString(), StrBuffer.GetSize(), 1 ))
		{
			HE_LOG( Error, TEXT( "Failed to serialize database %s" ), CharToTChar( GetName() ) );
			HE_ASSERT( false );
		}
	}
}
