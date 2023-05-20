#include "ScriptingPCH.h"

#include "SourceContext.h"

#include "LuaScript.h"


SourceContext::SourceContext()
	: m_pLuaSrcContext( nullptr )
{

}

SourceContext::~SourceContext()
{
	Teardown();
}

void SourceContext::Scr_DebugLog( const Char* Msg )
{
#if HE_DEBUG
	S_LOG( Log, TEXT( "%s" ), CharToTChar( Msg ) );
#endif
}

void SourceContext::Setup()
{
	if (m_pLuaSrcContext != nullptr)
		return;

	CreateLogger( GScriptEngineLogger, "Script Engine" );
	CreateLogger( GScriptLogger, "Script" );

	m_pLuaSrcContext = LuaPlus::LuaState::Create();
	HE_ASSERT( m_pLuaSrcContext != nullptr );

	BindLuaFunction( "DebugLog", (*this), &SourceContext::Scr_DebugLog );
}

void SourceContext::Teardown()
{
	if (m_pLuaSrcContext != nullptr)
	{
		LuaPlus::LuaState::Destroy( m_pLuaSrcContext );
		m_pLuaSrcContext = nullptr;
	}
}

bool SourceContext::RunScript( LuaScript& Script )
{
	m_pLuaSrcContext->DoString( Script.GetSourceCode() );

	LuaPlus::LuaObject Globals = m_pLuaSrcContext->GetGlobals();
	LuaPlus::LuaObject LuaUpdateFn = Globals.GetByName( "Update" );
	if (LuaUpdateFn.IsFunction())
	{
		LuaPlus::LuaFunctionVoid UpdateFn( LuaUpdateFn );
		UpdateFn();
	}
	else
		S_LOG( Warning, TEXT("Update function was not found in script: "), CharToTChar( Script.GetScriptName() ) );

	return true;
}
