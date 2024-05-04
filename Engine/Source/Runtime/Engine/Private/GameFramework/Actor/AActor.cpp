// Copyright 2021 Insight Interactive. All Rights Reserved.
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
				Output.Key( "ObjectName" );
				Output.String( TCharToChar( GetObjectName().c_str() ) );
				Output.Key( "ObjectGUID" );
				Output.String( GetGuid().ToString().CStr() );
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
						m_Components[i]->Serialize( Output );
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

	// Object Name
	char ObjectNameBuffer[32];
	JsonUtility::GetString( HObjectProps, "ObjectName", ObjectNameBuffer, sizeof( ObjectNameBuffer ) );
	SetObjectName( CharToTChar( ObjectNameBuffer ) );

	// GUID
	Char GuidStr[64];
	ZeroMemory( GuidStr, sizeof( GuidStr ) );
	JsonUtility::GetString( HObjectProps, "ObjectGUID", GuidStr, sizeof( GuidStr ) );
	SetGuid( FGUID::CreateFromString( GuidStr ) );

	// Loop over all the actor's components.
	const Char* StaticMeshKey = HE_STRINGIFY( HStaticMeshComponent );
	const Char* PointLightKey = HE_STRINGIFY( HPointLightComponent );
	const Char* SceneComponentKey = HE_STRINGIFY( HSceneComponent );
	const Char* PlaneColliderKey = HE_STRINGIFY( HPlaneColliderComponent );
	const Char* SphereColliderKey = HE_STRINGIFY( HSphereColliderComponent );
	const Char* CubeColliderKey = HE_STRINGIFY( HCubeColliderComponent );
	const Char* LuaScriptKey = HE_STRINGIFY( HLuaScriptComponent );
	const Char* CameraBoomKey = HE_STRINGIFY( HCameraBoomComponent );
	const Char* CapsuleColliderKey = HE_STRINGIFY( HCapsuleColliderComponent );
	const rapidjson::Value& ActorComponents = ActorProps["Components"];
	for (uint32 i = 0; i < ActorComponents.Size(); ++i)
	{
		const rapidjson::Value& CurrentComponent = ActorComponents[i];

		if (CurrentComponent.HasMember( StaticMeshKey ))
		{
			AddComponent<HStaticMeshComponent>( TEXT( "<Unnamed Static Mesh Component>" ) )
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
		else if (CurrentComponent.HasMember( PlaneColliderKey ))
		{
			AddComponent<HPlaneColliderComponent>( TEXT( "<Unnamed Plane Collider Component>" ) )
				->Deserialize( CurrentComponent[PlaneColliderKey] );
		}
		else if (CurrentComponent.HasMember( SphereColliderKey ))
		{
			AddComponent<HSphereColliderComponent>( TEXT( "<Unnamed Sphere Collider Component>" ) )
				->Deserialize( CurrentComponent[SphereColliderKey] );
		}
		else if (CurrentComponent.HasMember( CubeColliderKey ))
		{
			AddComponent<HCubeColliderComponent>( TEXT( "<Unnamed Cube Collider Component>" ) )
				->Deserialize( CurrentComponent[CubeColliderKey] );
		}
		else if (CurrentComponent.HasMember( CapsuleColliderKey ))
		{
			AddComponent<HCapsuleColliderComponent>( TEXT( "<Unnamed Capsule Collider Component>" ) )
				->Deserialize( CurrentComponent[CapsuleColliderKey] );
		}
		else if (CurrentComponent.HasMember( LuaScriptKey ))
		{
			AddComponent<HLuaScriptComponent>( TEXT( "<Unnamed Lua Script Component>" ) )
				->Deserialize( CurrentComponent[LuaScriptKey] );
		}
		else if (CurrentComponent.HasMember( CameraBoomKey ))
		{
			AddComponent<HCameraBoomComponent>( TEXT( "<Unnamed Camera Boom Component>" ) )
				->Deserialize( CurrentComponent[LuaScriptKey] );
		}
		else
		{
			HE_LOG( Error, TEXT( "Unrecognized component type when deserializing actor with name: %s" ), GetObjectName().c_str() );
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
	HE_LOG( Log, TEXT( "Selected Actor: %s" ), GetObjectName().c_str() );
}

#endif