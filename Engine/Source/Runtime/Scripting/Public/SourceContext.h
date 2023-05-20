#pragma once

#include "TSingleton.h"

#include "LuaPlus/LuaPlus.h"


class LuaScript;
class SourceContext;

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
	LuaPlus::LuaState* GetLuaState();

	// Run a lua script.
	bool RunScript( LuaScript& Script );
	// Bind a C function to able to call it from script.
	template <typename ClassType, typename CallbackType>
	bool BindLuaFunction( const char* ScriptSytaxName, ClassType& CallbackContext, CallbackType Callback);
	
private:
	void Scr_DebugLog( const Char* Msg );

private:
	LuaPlus::LuaState* m_pLuaSrcContext;

};

//
// Inline function implementations
//

FORCEINLINE bool SourceContext::IsReady() const
{
	return m_pLuaSrcContext != nullptr;
}

FORCEINLINE LuaPlus::LuaState* SourceContext::GetLuaState()
{
	HE_ASSERT( IsReady() );
	return m_pLuaSrcContext;
}

template <typename ClassType, typename CallbackType>
FORCEINLINE bool SourceContext::BindLuaFunction( const char* ScriptSytaxName, ClassType& CallbackContext, CallbackType Callback )
{
	m_pLuaSrcContext->GetGlobals().RegisterDirect( ScriptSytaxName, CallbackContext, Callback );

	return true;
}
