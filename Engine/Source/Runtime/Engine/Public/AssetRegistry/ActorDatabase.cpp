#include "EnginePCH.h"

#include "AssetRegistry/ActorDatabase.h"

#include "FileSystem.h"
#include "JsonUtility.h"
#include "StringHelper.h"


FActorDatabase::FActorDatabase()
	: FDatabaseInterface( "Actor Database" )
{

}

FActorDatabase::~FActorDatabase()
{

}

void FActorDatabase::Initialize( const Char* ActorDatabaseFile )
{
	// Load the texture cache.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( ActorDatabaseFile, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator itr = JsonDoc.MemberBegin();
			itr != JsonDoc.MemberEnd(); ++itr)
		{
			String ActorName = itr->name.GetString();
			String ActorPath = itr->value.GetString();
			HE_LOG( Log, TEXT( "Loading actor into database: %s with path %s" ), CharToTChar( ActorName ), CharToTChar( ActorPath ) );
			m_Data[ActorName] = ActorPath;
		}
	}
}

void FActorDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing texture database." ) );
	m_Data.clear();
}

void FActorDatabase::SerializeToFile_Implementation( const Char* Filepath )
{
	rapidjson::StringBuffer StrBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer( StrBuffer );

	// Prep the buffer.
	Writer.StartObject();
	{
		for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
		{
			Writer.Key( Iter->first.c_str() );
			Writer.String( Iter->second.c_str() );
		}
	}
	Writer.EndObject();

	// Write the file
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
