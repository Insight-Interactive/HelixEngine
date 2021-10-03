#pragma once

#include "TSingleton.h"


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

	const HName& GetGameName() const;

protected:
	FGameProject();
	virtual ~FGameProject();

	/*
		Loads a game project from *.hproject
	*/
	void Startup( const Char* HProjectFilpath );
	void SetProjectRootDirectory( const Char* ProjectRoot );

private:
	HName m_GameName;

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

FORCEINLINE const HName& FGameProject::GetGameName() const
{
	return m_GameName;
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
