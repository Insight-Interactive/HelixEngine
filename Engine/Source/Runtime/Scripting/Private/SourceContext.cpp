#include "ScriptingPCH.h"

#include "SourceContext.h"


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

	m_pLuaSrcContext = luaL_newstate();
	HE_ASSERT( m_pLuaSrcContext != nullptr );

	luaL_dostring( m_pLuaSrcContext, "x = 7257" );
	lua_getglobal( m_pLuaSrcContext, "x" );
	lua_Number num = lua_tonumber( m_pLuaSrcContext, 1 );
	
	printf( "lua says x = %d", (int)num );

}

void SourceContext::Teardown()
{
	if (m_pLuaSrcContext != nullptr)
	{
		lua_close( m_pLuaSrcContext );
		m_pLuaSrcContext = nullptr;
	}
}
