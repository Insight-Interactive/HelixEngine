#include "EnginePCH.h"

#include "AssetRegistry/MaterialDatabase.h"
#include "FileSystem.h"
#include "JsonUtility.h"


FMaterialDatabase::FMaterialDatabase()
	: FDatabaseInterface( "Material Database" )
{
}

FMaterialDatabase::~FMaterialDatabase()
{
}

void FMaterialDatabase::Initialize( const Char* MaterialDatabaseFile )
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

void FMaterialDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing material database." ) );
	m_Data.clear();
}

void FMaterialDatabase::SerializeToFile_Implementation( const Char* Filepath )
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
