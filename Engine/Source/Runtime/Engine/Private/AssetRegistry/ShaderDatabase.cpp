// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/ShaderDatabase.h"


FShaderDatabase::FShaderDatabase()
	: FDatabaseInterface( "Shader Database" )
{
}

FShaderDatabase::~FShaderDatabase()
{
}

void FShaderDatabase::Deserialize( const JsonUtility::ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* ShaderGuidStr = itr->name.GetString();
		FGUID ShaderGUID = FGUID::CreateFromString( ShaderGuidStr );
		String ShaderPath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading shader (GUID: %s) into database with path %s" ), CharToTChar( ShaderGuidStr ), CharToTChar( ShaderPath ) );

		m_Data[ShaderGUID] = ShaderPath;
	}
}

void FShaderDatabase::Serialize( JsonUtility::WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FShaderDatabase::Initialize()
{
}

void FShaderDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing shader database." ) );
	m_Data.clear();
}

void FShaderDatabase::SerializeToFile_Implementation( const Char* Filepath )
{
	rapidjson::StringBuffer StrBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> Writer( StrBuffer );

	Writer.StartObject();
	{
		for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
		{
			GUIDString GuidStr = Iter->first.ToString();
			Writer.Key( GuidStr.CStr() );
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
