#include "EnginePCH.h"

#include "AssetRegistry/MeshDatabase.h"


FMeshDatabase::FMeshDatabase()
	: FDatabaseInterface("Mesh Database")
{

}

FMeshDatabase::~FMeshDatabase()
{

}

void FMeshDatabase::Deserialize( const ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* MeshGuidStr = itr->name.GetString();
		FGUID MeshGUID = FGUID::CreateFromString( MeshGuidStr );
		String MeshPath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading mesh (GUID: %s) into database with path %s" ), CharToTChar( MeshGuidStr ), CharToTChar( MeshPath ) );

		m_Data[MeshGUID] = MeshPath;
	}
}

void FMeshDatabase::Serialize( WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FMeshDatabase::Initialize()
{
}

void FMeshDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing mesh database." ) );
	m_Data.clear();
}

void FMeshDatabase::SerializeToFile_Implementation( const Char* Filepath )
{
	rapidjson::StringBuffer StrBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer( StrBuffer );

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
