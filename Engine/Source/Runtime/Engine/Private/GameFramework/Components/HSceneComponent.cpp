// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HSceneComponent.h"

#include "GameFramework/Actor/AActor.h"
#include "Engine/Engine.h"
#include "LuaScriptVM.h"

HSceneComponent::HSceneComponent( FComponentInitArgs& InitArgs )
	: HActorComponent( InitArgs )
	, m_pParent( nullptr )
	, m_IsStatic( false )
{
	AActor* pOwner = GetOwner();
	if (pOwner->GetRootComponent() == nullptr)
		pOwner->SetRootComponent( this );
	else
		AttachTo( pOwner->GetRootComponent() );

	/*LuaScriptVM& LuaEngine = GEngine->GetScriptSubsystem();
	LuaEngine.BindLuaFunction( "Translate", *this, &HSceneComponent::Translate );
	LuaEngine.BindLuaFunction( "Rotate", *this, &HSceneComponent::Rotate );
	LuaEngine.BindLuaFunction( "Scale", *this, &HSceneComponent::Scale );*/
}

HSceneComponent::~HSceneComponent()
{
}

void HSceneComponent::Render( FCommandContext& GfxContext )
{
}

void HSceneComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HSceneComponent ) );
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize( Output );
		}
		Output.EndObject();

		// Transform properties.
		Output.StartObject();
		{
			JsonUtility::WriteTransform( Output, HE_STRINGIFY( m_Transform ), m_Transform );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HSceneComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	JsonUtility::GetTransform( Value, HE_STRINGIFY( HSceneComponent::m_Transform ), m_Transform );
}
