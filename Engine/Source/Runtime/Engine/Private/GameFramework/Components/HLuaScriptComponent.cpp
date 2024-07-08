#include "EnginePCH.h"

#include "GameFramework/Components/HLuaScriptComponent.h"

#include "AssetRegistry/AssetDatabase.h"
#include "LuaScriptVM.h"


HLuaScriptComponent::HLuaScriptComponent( FComponentInitArgs& InitArgs )
	: Super( InitArgs )
{

}

HLuaScriptComponent::~HLuaScriptComponent()
{

}

void HLuaScriptComponent::Tick( float DeltaTime ) 
{
	LuaScriptVM::GetInstance()->RunScript( *m_ScriptAsset.get() );

}

void HLuaScriptComponent::Serialize( JsonUtility::WriteContext& Output )
{
}

void HLuaScriptComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	Char ScriptBuffer[64];
	JsonUtility::GetString( Value, HE_STRINGIFY( m_ScriptAsset ), ScriptBuffer, sizeof( ScriptBuffer ) );
	m_ScriptAsset = FAssetDatabase::GetScript( ScriptBuffer );
}
