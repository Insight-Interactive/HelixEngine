// Copyright 2024 Insight Interactive. All Rights Reserved.
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

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	LuaScriptRef m_ScriptAsset;

};
