#pragma once

#include "GameFramework/Components/HColliderComponent.h"

#include "LuaScript.h"


HCOMPONENT()
class HLuaScriptComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HLuaScriptComponent )


protected:

	virtual void Tick( float DeltaTime ) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

private:
	LuaScriptRef m_ScriptAsset;

};