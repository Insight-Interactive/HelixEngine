// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/MaterialDatabase.h"
#include "..\..\Public\AssetRegistry\MaterialDatabase.h"


FMaterialDatabase::FMaterialDatabase()
	: FDatabaseInterface( "Material Database" )
{
}

FMaterialDatabase::~FMaterialDatabase()
{
}

void FMaterialDatabase::Deserialize( const ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		const Char* MaterialGuidStr = itr->name.GetString();
		FGUID MaterialGUID = FGUID::CreateFromString( MaterialGuidStr );
		String MaterialPath = itr->value.GetString();
		HE_LOG( Log, TEXT( "Loading material (GUID: %s) into database with path %s" ), CharToTChar( MaterialGuidStr ), CharToTChar( MaterialPath ) );

		m_Data[MaterialGUID] = MaterialPath;
	}
}

void FMaterialDatabase::Serialize( WriteContext& Writer )
{
	for (auto Iter = m_Data.begin(); Iter != m_Data.end(); Iter++)
	{
		Writer.Key( Iter->first.ToString().CStr() );
		Writer.String( Iter->second.c_str() );
	}
}

void FMaterialDatabase::Initialize()
{
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
