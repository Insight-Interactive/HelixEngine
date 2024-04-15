#include "ScriptingPCH.h"

#include "LuaScript.h"

#include "LuaScriptVM.h"


LuaScript::LuaScript()
{

}

LuaScript::LuaScript( const char* Filename )
{
	LoadSource( Filename );
}

LuaScript::~LuaScript()
{

}

LuaScript::LuaScript( const LuaScript& Other )
{
	m_SourceCode = Other.m_SourceCode;
}

void LuaScript::LoadSource( const char* Filepath )
{
	m_SourceCode->Load( Filepath, FUM_Read, CM_Text );
}

const char* LuaScript::GetSourceCode() 
{
	return (const char*)m_SourceCode->Data();
}

String LuaScript::GetScriptName() 
{
	return StringHelper::GetFilenameFromDirectoryNoExtension( m_SourceCode->GetFilepath() );
}
