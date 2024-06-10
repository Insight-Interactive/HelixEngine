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
	const String GetDefaultLevelPath() const;

	void GetProjectDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ProjectRoot );
	}

	void GetConfigDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ConfigDirectory );
	}

	/*
		Returns the full path for a piece of content located in the project's Content directory.
	*/
	void GetContentDirectoryFullPath( const char* AssetName, char* PathDestination, const uint32& DestinationLength )
	{
		AppendAssetToPath( AssetName, PathDestination, DestinationLength, m_ContentDirectory );
	}


	const HName& GetGameName() const;
	const HName& GetProjectName() const;

protected:
	FGameProject();
	virtual ~FGameProject();

	/*
		Loads a game project from *.hproject
	*/
	void Startup( const Char* HProjectFilpath );
	void SetProjectRootDirectory( const Char* ProjectRoot );

private:
	HName m_ProjectName;

	FileRef m_HProject;

	char m_ProjectRoot[HE_MAX_PATH];
	char m_ConfigDirectory[HE_MAX_PATH];
	char m_ContentDirectory[HE_MAX_PATH];
};

#undef AppendAssetToPath

FORCEINLINE const char* FGameProject::GetProjectRoot() const
{
	return m_ProjectRoot;
}
FORCEINLINE const char* FGameProject::GetConfigFolder() const
{
	return m_ConfigDirectory;
}

FORCEINLINE const char* FGameProject::GetContentFolder() const
{
	return m_ContentDirectory;
}

FORCEINLINE const HName& FGameProject::GetGameName() const
{
	return FApp::GetInstance()->GetName();
}

FORCEINLINE const HName& FGameProject::GetProjectName() const
{
	return m_ProjectName;
}

FORCEINLINE void FGameProject::SetProjectRootDirectory( const Char* ProjectRoot )
{
#if HE_STANDALONE && !HE_DEMO_GAME
	"Data\\";
	sprintf_s( m_ProjectRoot, sizeof( m_ProjectRoot ), "Data\\" );
#else	
	sprintf_s( m_ProjectRoot, sizeof( m_ProjectRoot ), ProjectRoot );
#endif
	strcpy_s( m_ContentDirectory, m_ProjectRoot );
	strcat_s( m_ContentDirectory, "Content\\" );

	strcpy_s( m_ConfigDirectory, m_ProjectRoot );
	strcat_s( m_ConfigDirectory, "Config\\" );
}
