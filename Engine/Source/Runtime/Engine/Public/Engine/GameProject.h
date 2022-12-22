#pragma once

#include "TSingleton.h"

#include "App/App.h"
#include "FileSystem.h"

/*
	Represents a game project that the engine is currently editing and/or playing.
*/
class FGameProject : public TSingleton<FGameProject>
{
	friend class HEngine;
public:
	const String& GetProjectRoot() const;
	const String& GetConfigFolder() const;
	const String& GetContentFolder() const;
	const String GetDefaultLevelPath() const;

	/*
		Returns the full path for a piece of content located in the project's content directory.
	*/
	String GetContentFullPath( const String& ContentSubDirectory );
	String GetConfigFileFullPath( const String& ConfigFileSubDirectory );

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

	String m_ProjectRoot;
	String m_ConfigDirectory;
	String m_ContentDirectory;
};

FORCEINLINE const String& FGameProject::GetProjectRoot() const
{
	return m_ProjectRoot;
}
FORCEINLINE const String& FGameProject::GetConfigFolder() const
{
	return m_ConfigDirectory;
}

FORCEINLINE const String& FGameProject::GetContentFolder() const
{
	return m_ContentDirectory;
}

FORCEINLINE String FGameProject::GetContentFullPath( const String& ContentSubDirectory )
{
	return GetContentFolder() + "/" + ContentSubDirectory;
}

FORCEINLINE String FGameProject::GetConfigFileFullPath( const String& ConfigFileSubDirectory )
{
	return GetConfigFolder() + "/" + ConfigFileSubDirectory;
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
	m_ProjectRoot = 
#if HE_STANDALONE
	"Data\\";
#else	
	ProjectRoot;
#endif
	m_ContentDirectory = m_ProjectRoot + "Content";
	m_ConfigDirectory = m_ProjectRoot + "Config";
}
