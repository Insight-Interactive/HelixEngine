// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/TextureDatabase.h"
#include "..\..\Public\AssetRegistry\TextureDatabase.h"


FTextureDatabase::FTextureDatabase()
	: FDatabaseInterface( "Texture Database")
{

}

FTextureDatabase::~FTextureDatabase()
{

}

void FTextureDatabase::Deserialize( const ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* TextureGuidStr = itr->name.GetString();
		FGUID TextureGUID = FGUID::CreateFromString( TextureGuidStr );
		String TexturePath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading texture (GUID: %s) into database with path %s" ), CharToTChar( TextureGuidStr ), CharToTChar( TexturePath ) );

		m_Data[TextureGUID] = TexturePath;
	}
}

void FTextureDatabase::Serialize( WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FTextureDatabase::Initialize()
{
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
