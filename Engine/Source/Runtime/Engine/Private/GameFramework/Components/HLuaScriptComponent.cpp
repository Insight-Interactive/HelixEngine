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

void HLuaScriptComponent::Serialize( WriteContext& Output )
{
}

void HLuaScriptComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0] );

	const rapidjson::Value& ScriptComponent = Value[1];

	Char ScriptGuidBuffer[64];
	ZeroMemory( ScriptGuidBuffer, sizeof( ScriptGuidBuffer ) );
	JsonUtility::GetString( ScriptComponent, HE_STRINGIFY( m_ScriptAsset ), ScriptGuidBuffer, sizeof( ScriptGuidBuffer ) );
	FGUID ScriptGuid = FGUID::CreateFromString( ScriptGuidBuffer );
	m_ScriptAsset = FAssetDatabase::GetScript( ScriptGuid );
}
