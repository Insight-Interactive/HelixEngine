// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "TSingleton.h"

#include "App/App.h"
#include "FileSystem.h"

#define AppendAssetToPath(AssetName, PathDestination, DestinationLength, RootDirectory)	\
HE_ASSERT( DestinationLength > 0 && DestinationLength <= HE_MAX_PATH );					\
HE_ASSERT( PathDestination != nullptr && AssetName != nullptr );						\
ZeroMemory( PathDestination, DestinationLength );										\
strcpy_s( PathDestination, DestinationLength, (const char*)RootDirectory );				\
strcat_s( PathDestination, DestinationLength, AssetName );								\

/*
	Represents a game project that the engine is currently editing and/or playing.
*/
class FGameProject : public TSingleton<FGameProject>
{
	friend class HEngine;
public:
	const char* GetProjectRoot() const;
	const char* GetConfigFolder() const;
	const char* GetContentFolder() const;
	const void GetDefaultLevelPath( FPath& outPath ) const;

	/*
		Returns the full path for a piece of content located in the Game project's root directory.
		HelixEngine/Game/AssetName
	*/
	void GetProjectDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ProjectRoot.m_Path );
	}

	/*
		Returns the full path for a piece of content located in the Game project's Config directory.
		HelixEngine/Game/Config/AssetName
	*/
	void GetConfigDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ConfigDirectory.m_Path );
	}

	/*
		Returns the full path for a piece of content located in the Game project's Content directory.
		HelixEngine/Game/Content/AssetName
	*/
	void GetContentDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ContentDirectory.m_Path );
	}


	const HName& GetGameName() const;
	const char* GetProjectName() const;

protected:
	FGameProject();
	virtual ~FGameProject();

	/*
		Loads a game project from *.hproject
	*/
	void Startup( FPath& HProjectFilpath );
	void SetProjectRootDirectory( const char* ProjectRoot );

private:
	char m_ProjectName[64];

	FPath m_ProjectRoot;
	FPath m_ConfigDirectory;
	FPath m_ContentDirectory;
};

#undef AppendAssetToPath

FORCEINLINE const char* FGameProject::GetProjectRoot() const
{
	return m_ProjectRoot.GetFullPath();
}
FORCEINLINE const char* FGameProject::GetConfigFolder() const
{
	return m_ConfigDirectory.GetFullPath();
}

FORCEINLINE const char* FGameProject::GetContentFolder() const
{
	return m_ContentDirectory.GetFullPath();
}

FORCEINLINE const HName& FGameProject::GetGameName() const
{
	return FApp::GetInstance()->GetName();
}

FORCEINLINE const char* FGameProject::GetProjectName() const
{
	return m_ProjectName;
}

FORCEINLINE void FGameProject::SetProjectRootDirectory( const char* ProjectRoot )
{
#if HE_STANDALONE && !HE_DEMO_GAME
	"Data\\";
	sprintf_s( m_ProjectRoot, sizeof( m_ProjectRoot ), "Data\\" );
#else	
	m_ProjectRoot.SetPath( ProjectRoot );
#endif
	m_ContentDirectory.SetPath( m_ProjectRoot.m_Path );
	m_ContentDirectory.Concat( "Content\\" );

	m_ConfigDirectory.SetPath( m_ProjectRoot.m_Path );
	m_ConfigDirectory.Concat( "Config\\" );
}
