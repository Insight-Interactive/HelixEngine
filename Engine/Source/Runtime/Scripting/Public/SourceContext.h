#pragma once

#include "TSingleton.h"


struct lua_State;

class LuaScript;
class SourceContext;

typedef int (ClientLuaFn)( lua_State* );

/*
	The global context in which all lua source code executes.
*/
class SCRIPT_API SourceContext : public TSingleton<SourceContext>
{
public:
	SourceContext();
	~SourceContext();

	void Setup();
	void Teardown();

	bool IsReady() const;
	lua_State* GetLuaState();

	// Run a lua script.
	bool RunScript( LuaScript& Script );
	// Bind a C function to able to call it from script.
	bool BindLuaFunction( ClientLuaFn Callback, const char* ScriptSytaxName );

private:
	lua_State* m_pLuaSrcContext;

};

//
// Inline function implementations
//

FORCEINLINE bool SourceContext::IsReady() const
{
	return m_pLuaSrcContext != nullptr;
}

FORCEINLINE lua_State * SourceContext::GetLuaState()
{
	HE_ASSERT( IsReady() );
	return m_pLuaSrcContext;
}
