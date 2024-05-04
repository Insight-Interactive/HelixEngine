// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "AssetRegistry/SerializeableInterface.h"

#include "GUID.h"
#include "StringHelper.h"


class FDatabaseInterface : public FSerializeableInterface
{
	friend class FAssetDatabase;
	friend class HEditorEngine;
public:
	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;
	void RegisterAsset( const FGUID& AssetGuid, const Char* Filepath );
	void UnRegisterAsset( const FGUID& AssetGuid );

	const String& GetValueByKey( const FGUID& Key ) const;
	bool GetIsDirty() const;
	const String& GetName() const;

protected:
	FDatabaseInterface( const String& Name = "Unnamed Database");
	virtual ~FDatabaseInterface();

	const String& GetInvalidAssetPath() const;
	void SerializeToFile( const Char* Filepath );
	void SetDirty( bool IsDirty );

	virtual void SerializeToFile_Implementation( const Char* Filepath ) = 0;

private:
	/*
		Debug name for the database.
	*/
	String m_DatabaseName;
	
	/*
		True if the database has been modified and has unsaved changes. False if not.
	*/
	bool m_IsDirty;
	
	/*
		Const string to indicate invalid paths.s
	*/
	static const String SInvalidAssetPath;

protected:
	std::unordered_map<FGUID, String, FGUIDHashFunction> m_Data;

};

//
// Inline function implementations
//

FORCEINLINE void FDatabaseInterface::RegisterAsset( const FGUID& AssetGuid, const Char* Filepath )
{
	auto InsertResult = m_Data.try_emplace( AssetGuid, String( Filepath ) );
	if (InsertResult.second)
	{
		GUIDString GuidStr = AssetGuid.ToString();
		HE_LOG( Warning, TEXT( "Registered asset Guid(\"%s\") into database (\"%s\")." ), CharToTChar( (const char*)GuidStr.CStr()), CharToTChar( m_DatabaseName ) );
		SetDirty( true );
	}
}

FORCEINLINE void FDatabaseInterface::UnRegisterAsset( const FGUID& AssetGuid )
{
	auto Iter = m_Data.find( AssetGuid );
	if (Iter != m_Data.end())
	{
		m_Data.erase( Iter );
	}
	else
	{
		GUIDString GuidStr = AssetGuid.ToString();
		HE_LOG( Warning, TEXT( "Trying to unregister asset (\"%s\") from database (\"%s\") that does not exist!" ), CharToTChar( (const char*)GuidStr.CStr() ), CharToTChar( m_DatabaseName ) );
		HE_ASSERT( false );
	}
}

FORCEINLINE const String& FDatabaseInterface::GetInvalidAssetPath() const
{
	return SInvalidAssetPath;
}

FORCEINLINE void FDatabaseInterface::SerializeToFile( const Char* Filepath )
{
	if (GetIsDirty())
	{
		SerializeToFile_Implementation(Filepath);
		SetDirty( false );
	}
}

FORCEINLINE const String& FDatabaseInterface::GetValueByKey( const FGUID& Key ) const
{
	auto Iter = m_Data.find( Key );
	if (Iter != m_Data.end())
	{
		return Iter->second;
	}
	else
	{
		HE_LOG( Error, TEXT( "Trying to find a item that has not been registered with the database. Has it been imported into the project?" ) );
		HE_ASSERT( false );
		return GetInvalidAssetPath();
	}
}

FORCEINLINE const String& FDatabaseInterface::GetName() const
{
	return m_DatabaseName;
}

FORCEINLINE bool FDatabaseInterface::GetIsDirty() const
{
	return m_IsDirty;
}

FORCEINLINE void FDatabaseInterface::SetDirty( bool IsDirty )
{
	m_IsDirty = IsDirty;
}
