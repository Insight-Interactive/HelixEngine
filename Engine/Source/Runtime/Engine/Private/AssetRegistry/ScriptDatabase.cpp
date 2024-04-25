// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/ScriptDatabase.h"


FScriptDatabase::FScriptDatabase()
{

}

FScriptDatabase::~FScriptDatabase()
{

}

void FScriptDatabase::Deserialize( const JsonUtility::ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* ScriptGuidStr = itr->name.GetString();
		FGUID ScriptGUID = FGUID::CreateFromString( ScriptGuidStr );
		String ScriptPath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading script (GUID: %s) into database with path %s" ), CharToTChar( ScriptGuidStr ), CharToTChar( ScriptPath ) );

		m_Data[ScriptGUID] = ScriptPath;
	}
}

void FScriptDatabase::Serialize( JsonUtility::WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FScriptDatabase::Initialize()
{
}

void FScriptDatabase::UnInitialize()
{
	HE_LOG( Log, TEXT( "Clearing mesh database." ) );
	m_Data.clear();
}

void FScriptDatabase::SerializeToFile_Implementation( const Char* Filepath )
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