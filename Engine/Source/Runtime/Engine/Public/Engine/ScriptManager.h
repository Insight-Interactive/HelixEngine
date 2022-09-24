#pragma once

#include "LuaScript.h"
#include "CriticalSection.h"


class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();
	
	LuaScriptRef FindOrLoadScript( const String& Filepath );

	bool ScriptExists( const String& Name ) const;
	inline LuaScriptRef GetScriptByName( const String& Name );

private:
	CriticalSection m_MapMutex;
	std::map<String, LuaScriptRef> m_ScriptCache;

};

extern ScriptManager GScriptManager;


//
// Inline function implementations
//

inline bool ScriptManager::ScriptExists( const String& Name ) const
{
	auto Iter = m_ScriptCache.find( Name );
	return Iter != m_ScriptCache.end();
}

inline LuaScriptRef ScriptManager::GetScriptByName( const String& Name )
{
	auto Iter = m_ScriptCache.find( Name );
	if (Iter != m_ScriptCache.end())
	{
		return m_ScriptCache.at( Name );
	}
	else
	{
		HE_ASSERT( false );
	}

	return NULL;
}