#pragma once

#include "StringHelper.h"


class DatabaseInterface
{
	friend class AssetDatabase;
public:
	virtual void Initialize( const Char* MaterialDatabaseFile ) = 0;
	virtual void UnInitialize() = 0;
	void RegisterAsset( const Char* TextureName, const Char* Filepath );
	void UnRegisterAsset( const Char* TextureName );

	const String& GetValueByKey( const String& Key ) const;
	bool GetIsDirty() const;
	const String& GetName() const;

protected:
	DatabaseInterface( const String& Name );
	virtual ~DatabaseInterface();

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
	static const String s_InvalidAssetPath;

protected:
	std::unordered_map<String, String> m_Data;

};

//
// Inline function implementations
//

inline void DatabaseInterface::RegisterAsset( const Char* Name, const Char* Filepath )
{
	auto InsertResult = m_Data.try_emplace( String( Name ), String( Filepath ) );
	if (InsertResult.second)
	{
		HE_LOG( Warning, TEXT( "Registered asset (\"%s\") into database (\"%s\")." ), CharToTChar( Name ), CharToTChar( m_DatabaseName ) );
		SetDirty( true );
	}
}

inline void DatabaseInterface::UnRegisterAsset( const Char* Name )
{
	auto Iter = m_Data.find( Name );
	if (Iter != m_Data.end())
	{
		m_Data.erase( Iter );
	}
	else
	{
		HE_LOG( Warning, TEXT( "Trying to unregister asset (\"%s\") from database (\"%s\") that does not exist!" ), CharToTChar( Name ), CharToTChar( m_DatabaseName ) );
		HE_ASSERT( false );
	}
}

inline const String& DatabaseInterface::GetInvalidAssetPath() const
{
	return s_InvalidAssetPath;
}

inline void DatabaseInterface::SerializeToFile( const Char* Filepath )
{
	if (GetIsDirty())
	{
		SerializeToFile_Implementation(Filepath);
		SetDirty( false );
	}
}

inline const String& DatabaseInterface::GetValueByKey( const String& Key ) const
{
	auto Iter = m_Data.find( Key );
	if (Iter != m_Data.end())
	{
		return Iter->second;
	}
	else
	{
		HE_LOG( Error, TEXT( "Trying to find a item that has not been registered with the database. Has it been imported into the project?" ) );
		return GetInvalidAssetPath();
	}
}

inline const String& DatabaseInterface::GetName() const
{
	return m_DatabaseName;
}

inline bool DatabaseInterface::GetIsDirty() const
{
	return m_IsDirty;
}

inline void DatabaseInterface::SetDirty( bool IsDirty )
{
	m_IsDirty = IsDirty;
}
