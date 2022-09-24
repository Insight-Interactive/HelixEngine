#include "ScriptingPCH.h"

#include "SourceContext.h"

#include "LuaScript.h"
#include "../LuaNativeFunctions.h"


SourceContext::SourceContext()
	: m_pLuaSrcContext( nullptr )
{

}

SourceContext::~SourceContext()
{
	Teardown();
}

void SourceContext::Setup()
{
	if (m_pLuaSrcContext != nullptr)
		return;

	CreateLogger( GScriptEngineLogger, "Script Engine" );
	CreateLogger( GScriptLogger, "Script" );

	m_pLuaSrcContext = luaL_newstate();
	HE_ASSERT( m_pLuaSrcContext != nullptr );

	BindLuaFunction( Scr_DebugLog, "DebugLog" );

	//	const char* LUA_SCR = R"(
	//		
	//x = 7257
	//
	//function MyFunction()
	//
	//end
	//		)";
	//	
	//	int err = luaL_dostring( m_pLuaSrcContext, LUA_SCR );
	//
	//	//lua_getglobal( m_pLuaSrcContext, "x" );
	//	//lua_Number num = lua_tonumber( m_pLuaSrcContext, 1 );	
	//	//printf( "lua says x = %d", (int)num );
	//	lua_getglobal( m_pLuaSrcContext, "MyFunction" );
	//	if (lua_isfunction( m_pLuaSrcContext, -1 ))
	//	{
	//		int Status = lua_pcall( m_pLuaSrcContext, 0, 0, 0 );
	//	}


}

void SourceContext::Teardown()
{
	if (m_pLuaSrcContext != nullptr)
	{
		lua_close( m_pLuaSrcContext );
		m_pLuaSrcContext = nullptr;
	}
}

bool SourceContext::RunScript( LuaScript& Script )
{
	int Err = luaL_dostring( m_pLuaSrcContext, Script.GetSourceCode() );
	if (Err != LUA_OK)
	{
		const char* ErrMsg = lua_tostring( m_pLuaSrcContext, -1 );
		SE_LOG( Error, TEXT( "Script compile error: \"%s\" -> %s" ), CharToTChar( Script.GetScriptName() ), CharToTChar( ErrMsg ) );
		return false;
	}

	lua_getglobal( m_pLuaSrcContext, "Update" );
	if (lua_isfunction( m_pLuaSrcContext, -1 ))
	{
		int Status = lua_pcall( m_pLuaSrcContext, 0, 0, 0 );
		if (Status != LUA_OK)
		{
			const char* ErrMsg = lua_tostring( m_pLuaSrcContext, -1 );
			SE_LOG( Error, TEXT( "Script runtime error: \"%s\" -> %s" ), CharToTChar( Script.GetScriptName() ), CharToTChar( ErrMsg ) );
			return false;
		}
	}

	return true;
}

bool SourceContext::BindLuaFunction( ClientLuaFn Callback, const char* ScriptSytaxName )
{
	lua_pushcfunction( m_pLuaSrcContext, Callback );
	lua_setglobal( m_pLuaSrcContext, ScriptSytaxName );

	return true;
}
