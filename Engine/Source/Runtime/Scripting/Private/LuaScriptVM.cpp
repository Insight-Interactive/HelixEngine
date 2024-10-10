#include "ScriptingPCH.h"

#include "LuaScriptVM.h"

#include "LuaScript.h"


LuaScriptVM::LuaScriptVM()
	: m_pLuaState( nullptr )
{

}

LuaScriptVM::~LuaScriptVM()
{
	Teardown();
}

void LuaScriptVM::Scr_DebugLog( const Char* Msg )
{
#if HE_DEBUG
	S_LOG( Log, "%s", Msg );
#endif
}

void LuaScriptVM::Setup()
{
	if (m_pLuaState != nullptr)
		return;

	CreateLogger( GScriptEngineLogger, "Script Engine" );
	CreateLogger( GScriptLogger, "Script" );

	m_pLuaState = LuaPlus::LuaState::Create();
	HE_ASSERT( m_pLuaState != nullptr );

	BindLuaFunction( "DebugLog", (*this), &LuaScriptVM::Scr_DebugLog );
}

void LuaScriptVM::Teardown()
{
	if (m_pLuaState != nullptr)
	{
		LuaPlus::LuaState::Destroy( m_pLuaState );
		m_pLuaState = nullptr;
	}
}

bool LuaScriptVM::RunScript( LuaScript& Script )
{
	m_pLuaState->DoString( Script.GetSourceCode() );

	LuaPlus::LuaObject Globals = m_pLuaState->GetGlobals();
	LuaPlus::LuaObject LuaUpdateFn = Globals.GetByName( "Update" );
	if (LuaUpdateFn.IsFunction())
	{
		LuaPlus::LuaFunctionVoid UpdateFn( LuaUpdateFn );
		UpdateFn();
	}
	else
		S_LOG( Warning, "Update function was not found in script: ", CharToTChar( Script.GetScriptName() ) );

	return true;
}
