#include "EnginePCH.h"

#include "AssetRegistry/ActorDatabase.h"


FActorDatabase::FActorDatabase()
	: FDatabaseInterface( "Actor Database" )
{

}

FActorDatabase::~FActorDatabase()
{

}

void FActorDatabase::Deserialize( const ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* ActorGuidStr = itr->name.GetString();
		FGUID ActorGUID = FGUID::CreateFromString( ActorGuidStr );
		String ActorPath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading actor (GUID: %s) into database with path %s" ), CharToTChar( ActorGuidStr ), CharToTChar( ActorPath ) );

		m_Data[ActorGUID] = ActorPath;
	}
}

void FActorDatabase::Serialize( WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FActorDatabase::Initialize()
{
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
			GUIDString GuidStr = Iter->first.ToString();
			Writer.Key( (const char*)GuidStr.CStr() );
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
