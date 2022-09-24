#include "EnginePCH.h"

#include "Engine/ScriptManager.h"


ScriptManager GScriptManager;

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{

}

LuaScriptRef ScriptManager::FindOrLoadScript( const String& Filepath )
{
	String MeshName = StringHelper::GetFilenameFromDirectoryNoExtension( Filepath );
	if (ScriptExists( MeshName ))
		return GetScriptByName( MeshName );

	LuaScriptRef NewScript = std::make_shared<LuaScript>();
	NewScript->LoadSource( Filepath.c_str() );

	ScopedCriticalSection Guard( m_MapMutex );
	m_ScriptCache[MeshName].reset();
	m_ScriptCache[MeshName] = NewScript;
	return m_ScriptCache[MeshName];
}
