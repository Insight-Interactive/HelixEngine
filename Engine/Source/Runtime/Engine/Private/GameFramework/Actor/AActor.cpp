// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/AActor.h"

#include "ICommandContext.h"
#include "GameFramework/Components/HActorComponent.h"
#include "GameFramework/Components/HStaticMeshComponent.h"
#include "GameFramework/Components/HPointLightComponent.h"
#include "GameFramework/Components/HSceneComponent.h"
#include "AssetRegistry/AssetDatabase.h"
#include "JsonUtility.h"


AActor::AActor( HWorld* pWorld, const HName& Name )
	: HObject( Name )
	, m_pOwningWorld( pWorld )
{

}

AActor::~AActor()
{

}

void AActor::Serialize( rapidjson::Value& Value )
{

}

void AActor::Deserialize( const rapidjson::Value& Value ) 
{
	const rapidjson::Value& HObjectProps = Value[0];
	const rapidjson::Value& ActorProps = Value[1];

	// Object Name
	char ObjectNameBuffer[32];
	JsonUtility::GetString( HObjectProps, "ObjectName", ObjectNameBuffer, sizeof( ObjectNameBuffer ) );
	SetObjectName( CharToTChar( ObjectNameBuffer ) );

	// Loop over all the actor's components.
	const Char* StaticMeshKey		= "StaticMesh";
	const Char* PointLightKey		= "PointLight";
	const Char* SceneComponentKey	= "SceneComponent";
	const rapidjson::Value& ActorComponents = ActorProps["Components"];
	for (uint32 i = 0; i < ActorComponents.Size(); ++i)
	{
		const rapidjson::Value& CurrentComponent = ActorComponents[i];

		if (CurrentComponent.HasMember( StaticMeshKey ))
		{
			AddComponent<HStaticMeshComponent>( TEXT("<Unnamed Static Mesh Component>") )
				->Deserialize( CurrentComponent[StaticMeshKey] );
		}
		else if (CurrentComponent.HasMember( PointLightKey ))
		{
			AddComponent<HPointLightComponent>( TEXT( "<Unnamed Point Light Component>" ) )
				->Deserialize( CurrentComponent[PointLightKey] );
		}
		else if (CurrentComponent.HasMember( SceneComponentKey ))
		{
			AddComponent<HSceneComponent>( TEXT( "<Unnamed Scene Component>" ) )
				->Deserialize( CurrentComponent[SceneComponentKey] );
		}
	}
}

void AActor::RemoveAllComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->OnDetach();
		m_Components[i]->OnDestroy();
		delete m_Components[i];
	}
}

void AActor::BeginPlay()
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->BeginPlay();
	}
}

void AActor::Tick(float DeltaTime)
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Tick(DeltaTime);
	}
}

void AActor::Render(ICommandContext& GfxContext)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Render(GfxContext);
	}
}


#if HE_WITH_EDITOR

void AActor::OnEditorSelected()
{
	HE_LOG( Log, TEXT( "Selected Actor: %s" ), GetObjectName().c_str() );
}

#endif