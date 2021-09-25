#include "EnginePCH.h"

#include "AssetRegistry/MeshDatabase.h"

#include "FileSystem.h"
#include "JsonUtility.h"


MeshDatabase::MeshDatabase()
	: DatabaseInterface("Mesh Database")
{

}

MeshDatabase::~MeshDatabase()
{

}

void MeshDatabase::Initialize( const Char* MeshDatabaseFile )
{
	// Load the mesh cache.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( MeshDatabaseFile, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator itr = JsonDoc.MemberBegin();
			itr != JsonDoc.MemberEnd(); ++itr)
		{
			String MeshName = itr->name.GetString();
			String MeshPath = itr->value.GetString();
			HE_LOG( Log, TEXT( "Loading material into database: %s with path %s" ), CharToTChar( MeshName ), CharToTChar( MeshPath ) );
			m_Data[MeshName] = MeshPath;
		}
	}
}

void MeshDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing mesh database." ) );
	m_Data.clear();
}

void MeshDatabase::SerializeToFile_Implementation( const Char* Filepath )
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
