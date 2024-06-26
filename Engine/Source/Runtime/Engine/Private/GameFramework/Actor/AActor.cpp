// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/AActor.h"

#include "GameFramework/Components/HActorComponent.h"
#include "GameFramework/Components/HStaticMeshComponent.h"
#include "GameFramework/Components/HPointLightComponent.h"
#include "GameFramework/Components/HPlaneColliderComponent.h"
#include "GameFramework/Components/HSphereColliderComponent.h"
#include "GameFramework/Components/HCubeColliderComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"
#include "GameFramework/Components/HLuaScriptComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"
#include "GameFramework/Components/HSceneComponent.h"


AActor::AActor( FActorInitArgs& InitArgs )
	: HObject( InitArgs.Name )
	, m_pRootComponent( nullptr )
	, m_pOwningWorld( CCast<HWorld*>( InitArgs.pWorld ) )
	, m_IsDynamicInstance( InitArgs.bIsDynamicInstance )
{
}

AActor::~AActor()
{

}

void AActor::Serialize( JsonUtility::WriteContext& Output )
{
	if (m_IsDynamicInstance)
		return;

	Output.StartObject();
	{
		Output.Key( HE_STRINGIFY( AActor ) );
		Output.StartArray();
		{
			Output.StartObject();
			{
				Super::Serialize( Output );
			}
			Output.EndObject();

			Output.StartObject();
			{
				Output.Key( "Components" );
				Output.StartArray();
				{
					for (uint32 i = 0; i < m_Components.size(); ++i)
					{
						Output.StartObject();
						{
							Output.Key( "ComponentType" );
							Output.String( m_Components[i]->GetComponenetStaticName() );
							m_Components[i]->Serialize( Output );
						}
						Output.EndObject();
					}
				}
				Output.EndArray();
			}
			Output.EndObject();

		}
		Output.EndArray();
	}
	Output.EndObject();
}

void AActor::Deserialize( const JsonUtility::ReadContext& Value )
{
	enum
	{
		kHObjectProps = 0,
		kActorProps = 1,
	};
	const rapidjson::Value& HObjectProps = Value[kHObjectProps];
	const rapidjson::Value& ActorProps = Value[kActorProps];

	HObject::Deserialize( HObjectProps );

	// Loop over all the actor's components.
	const StringHashValue StaticMeshKey = StringHash( HE_STRINGIFY( HStaticMeshComponent ) );
	const StringHashValue PointLightKey = StringHash( HE_STRINGIFY( HPointLightComponent ) );
	const StringHashValue SceneComponentKey = StringHash( HE_STRINGIFY( HSceneComponent ) );
	const StringHashValue PlaneColliderKey = StringHash( HE_STRINGIFY( HPlaneColliderComponent ) );
	const StringHashValue SphereColliderKey = StringHash( HE_STRINGIFY( HSphereColliderComponent ) );
	const StringHashValue CubeColliderKey = StringHash( HE_STRINGIFY( HCubeColliderComponent ) );
	const StringHashValue LuaScriptKey = StringHash( HE_STRINGIFY( HLuaScriptComponent ) );
	const StringHashValue CameraBoomKey = StringHash( HE_STRINGIFY( HCameraBoomComponent ) );
	const StringHashValue CapsuleColliderKey = StringHash( HE_STRINGIFY( HCapsuleColliderComponent ) );
	const rapidjson::Value& ActorComponents = ActorProps[ "Components" ];
	Char TypeBuffer[64];
	for (uint32 i = 0; i < ActorComponents.Size(); ++i)
	{
		const rapidjson::Value& CurrentComponent = ActorComponents[i];

		JsonUtility::GetString( CurrentComponent, "ComponentType", TypeBuffer, sizeof( TypeBuffer ));
		HE_ASSERT( TypeBuffer != "\0" ); // All components MUST have "ComponentType" KVP!
		StringHashValue ComponentTypeHash = StringHash( TypeBuffer );

		if ( ComponentTypeHash == StaticMeshKey )
		{
			AddComponent<HStaticMeshComponent>( "Unnamed Static Mesh Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == PointLightKey )
		{
			AddComponent<HPointLightComponent>( "<Unnamed Point Light Component>" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == SceneComponentKey )
		{
			AddComponent<HSceneComponent>( "Unnamed Scene Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == PlaneColliderKey )
		{
			AddComponent<HPlaneColliderComponent>( "Unnamed Plane Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == SphereColliderKey )
		{
			AddComponent<HSphereColliderComponent>( "Unnamed Sphere Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CubeColliderKey )
		{
			AddComponent<HCubeColliderComponent>( "Unnamed Cube Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CapsuleColliderKey )
		{
			AddComponent<HCapsuleColliderComponent>( "Unnamed Capsule Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == LuaScriptKey )
		{
			AddComponent<HLuaScriptComponent>( "Unnamed Lua Script Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CameraBoomKey )
		{
			AddComponent<HCameraBoomComponent>( "Unnamed Camera Boom Component" )
				->Deserialize( CurrentComponent );
		}
		else
		{
			HE_LOG( Error, TEXT( "Unrecognized component type when deserializing actor with name: %s" ), GetObjectName() );
			HE_ASSERT( false );
		}
	}
}

void AActor::OnDeserializeComplete()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->OnOwnerDeserializeComplete();
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
	m_Components.clear();
}

void AActor::BeginPlay()
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->BeginPlay();
	}
}

void AActor::Tick( float DeltaTime )
{
	for (uint64 i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Tick( DeltaTime );
	}
}

void AActor::Render( FCommandContext& GfxContext )
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Render( GfxContext );
	}
}


#if HE_WITH_EDITOR

void AActor::OnEditorSelected()
{
	HE_LOG( Log, TEXT( "Selected Actor: %s" ), GetObjectName() );
}

#endif