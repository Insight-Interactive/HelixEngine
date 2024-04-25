#pragma once

#include "TSingleton.h"

#include "LuaPlus/LuaPlus.h"


class LuaScript;

/*
	The global context in which all lua source code executes.
*/
class SCRIPT_API LuaScriptVM : public TSingleton<LuaScriptVM>
{
public:
	LuaScriptVM();
	~LuaScriptVM();

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
	LuaPlus::LuaState* m_pLuaState;

};

//
// Inline function implementations
//

FORCEINLINE bool LuaScriptVM::IsReady() const
{
	return m_pLuaState != nullptr;
}

FORCEINLINE LuaPlus::LuaState* LuaScriptVM::GetLuaState()
{
	HE_ASSERT( IsReady() );
	return m_pLuaState;
}

template <typename ClassType, typename CallbackType>
FORCEINLINE bool LuaScriptVM::BindLuaFunction( const char* ScriptSytaxName, ClassType& CallbackContext, CallbackType Callback )
{
	m_pLuaState->GetGlobals().RegisterDirect( ScriptSytaxName, CallbackContext, Callback );

	return true;
}
