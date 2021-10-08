#include "EnginePCH.h"

#include "AssetRegistry/TextureDatabase.h"

#include "FileSystem.h"
#include "JsonUtility.h"
#include "StringHelper.h"


FTextureDatabase::FTextureDatabase()
	: FDatabaseInterface( "Texture Database")
{

}

FTextureDatabase::~FTextureDatabase()
{

}

void FTextureDatabase::Initialize( const Char* TextureDatabaseFile )
{
	// Load the texture cache.
	rapidjson::Document JsonDoc;
	FileRef JsonSource( TextureDatabaseFile, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( JsonSource, JsonDoc );
	if (JsonDoc.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator itr = JsonDoc.MemberBegin();
			itr != JsonDoc.MemberEnd(); ++itr)
		{
			String TexName = itr->name.GetString();
			String TexPath = itr->value.GetString();
			HE_LOG( Log, TEXT( "Loading texture into database: %s with path %s" ), CharToTChar( TexName ), CharToTChar( TexPath ) );
			m_Data[TexName] = TexPath;
		}
	}
}

void FTextureDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing texture database." ) );
	m_Data.clear();
}

void FTextureDatabase::SerializeToFile_Implementation( const Char* Filepath )
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
