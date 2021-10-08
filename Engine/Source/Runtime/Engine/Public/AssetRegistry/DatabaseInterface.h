#pragma once

#include "StringHelper.h"


class FDatabaseInterface
{
	friend class FAssetDatabase;
public:
	virtual void Initialize( const Char* MaterialDatabaseFile ) = 0;
	virtual void UnInitialize() = 0;
	void RegisterAsset( const Char* TextureName, const Char* Filepath );
	void UnRegisterAsset( const Char* TextureName );

	const String& GetValueByKey( const String& Key ) const;
	bool GetIsDirty() const;
	const String& GetName() const;

protected:
	FDatabaseInterface( const String& Name );
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
	std::unordered_map<String, String> m_Data;

};

//
// Inline function implementations
//

inline void FDatabaseInterface::RegisterAsset( const Char* Name, const Char* Filepath )
{
	auto InsertResult = m_Data.try_emplace( String( Name ), String( Filepath ) );
	if (InsertResult.second)
	{
		HE_LOG( Warning, TEXT( "Registered asset (\"%s\") into database (\"%s\")." ), CharToTChar( Name ), CharToTChar( m_DatabaseName ) );
		SetDirty( true );
	}
}

inline void FDatabaseInterface::UnRegisterAsset( const Char* Name )
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

inline const String& FDatabaseInterface::GetInvalidAssetPath() const
{
	return SInvalidAssetPath;
}

inline void FDatabaseInterface::SerializeToFile( const Char* Filepath )
{
	if (GetIsDirty())
	{
		SerializeToFile_Implementation(Filepath);
		SetDirty( false );
	}
}

inline const String& FDatabaseInterface::GetValueByKey( const String& Key ) const
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

inline const String& FDatabaseInterface::GetName() const
{
	return m_DatabaseName;
}

inline bool FDatabaseInterface::GetIsDirty() const
{
	return m_IsDirty;
}

inline void FDatabaseInterface::SetDirty( bool IsDirty )
{
	m_IsDirty = IsDirty;
}
