// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "AssetRegistry/ActorSerializer.h"

#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Components/HActorComponent.h"
#include "GameFramework/Components/HStaticMeshComponent.h"
#include "GameFramework/Components/HPointLightComponent.h"
#include "GameFramework/Components/HPlaneColliderComponent.h"
#include "GameFramework/Components/HSphereColliderComponent.h"
#include "GameFramework/Components/HCubeColliderComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"
#include "GameFramework/Components/HMeshColliderComponent.h"
#include "GameFramework/Components/HLuaScriptComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"


/*static*/ void FActorSerializer::DeserializeActorOverrides( AActor& OutActor, const JsonUtility::ReadContext& InstanceOverrides )
{
	enum
	{
		kActorOverries = 0,
		kComponentOverrides = 1,
	};
	const rapidjson::Value& ActorOverrides = InstanceOverrides[kActorOverries];
	const rapidjson::Value& ComponenetOverrides = InstanceOverrides[kComponentOverrides];

	OutActor.Deserialize( ActorOverrides );

	for (auto ComponentItr = ComponenetOverrides.MemberBegin(); ComponentItr != ComponenetOverrides.MemberEnd(); ++ComponentItr)
	{
		const char* ComponentGuidStr = ComponentItr->name.GetString();
		FGUID ComponenetGuid( ComponentGuidStr );
		HActorComponent* pComponent = OutActor.GetComponentByGuid( ComponenetGuid );
		if (pComponent)
			pComponent->Deserialize( ComponentItr->value[0] );
		else
		{
			HE_LOG( Warning, TEXT( "Trying to deserialize overrides of a componenet on actor (%s) that doesn't exist with GUID: %s" ), CharToTChar( OutActor.GetObjectName() ), CharToTChar( ComponentGuidStr ) );
			HE_ASSERT( false );
		}
	}
}

/*static*/ void FActorSerializer::DeserializeActor( AActor& OutActor, const JsonUtility::ReadContext& Value )
{
	enum
	{
		kHObjectProps	= 0,
		kActorProps		= 1,
	};
	const rapidjson::Value& HObjectProps = Value[kHObjectProps];
	const rapidjson::Value& ActorProps = Value[kActorProps];

	OutActor.Deserialize( HObjectProps );

	static StringHashValue StaticMeshKey;
	static StringHashValue PointLightKey;
	static StringHashValue PlaneColliderKey;
	static StringHashValue SphereColliderKey;
	static StringHashValue CubeColliderKey;
	static StringHashValue LuaScriptKey;
	static StringHashValue CameraBoomKey;
	static StringHashValue CapsuleColliderKey;
	static StringHashValue MeshColliderKey;
	static bool Init = false;
	if (!Init)
	{
		StaticMeshKey = StringHash( HE_STRINGIFY( HStaticMeshComponent ) );
		PointLightKey = StringHash( HE_STRINGIFY( HPointLightComponent ) );
		PlaneColliderKey = StringHash( HE_STRINGIFY( HPlaneColliderComponent ) );
		SphereColliderKey = StringHash( HE_STRINGIFY( HSphereColliderComponent ) );
		CubeColliderKey = StringHash( HE_STRINGIFY( HCubeColliderComponent ) );
		LuaScriptKey = StringHash( HE_STRINGIFY( HLuaScriptComponent ) );
		CameraBoomKey = StringHash( HE_STRINGIFY( HCameraBoomComponent ) );
		CapsuleColliderKey = StringHash( HE_STRINGIFY( HCapsuleColliderComponent ) );
		MeshColliderKey = StringHash( HE_STRINGIFY( HMeshColliderComponent ) );
		Init = true;
	}

	// Loop over all the actor's components.
	const rapidjson::Value& ActorComponents = ActorProps[ "Components" ];
	Char StrBuffer[64];
	for (uint32 i = 0; i < ActorComponents.Size(); ++i)
	{
		const rapidjson::Value& CurrentComponent = ActorComponents[i];
		if (CurrentComponent.ObjectEmpty())
			continue;

		ZeroMemory( StrBuffer, sizeof( StrBuffer ) );

		JsonUtility::GetString( CurrentComponent, "ComponentType", StrBuffer, sizeof( StrBuffer ) );
		HE_ASSERT( strcmp( StrBuffer, "" ) != 0 ); // All components MUST have "ComponentType" KVP!
		StringHashValue ComponentTypeHash = StringHash( StrBuffer );
		if (ComponentTypeHash == StaticMeshKey)
		{
			OutActor.AddComponent<HStaticMeshComponent>( "Unnamed Static Mesh Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == PointLightKey)
		{
			OutActor.AddComponent<HPointLightComponent>( "Unnamed Point Light Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == PlaneColliderKey)
		{
			OutActor.AddComponent<HPlaneColliderComponent>( "Unnamed Plane Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == SphereColliderKey)
		{
			OutActor.AddComponent<HSphereColliderComponent>( "Unnamed Sphere Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CubeColliderKey)
		{
			OutActor.AddComponent<HCubeColliderComponent>( "Unnamed Cube Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CapsuleColliderKey)
		{
			OutActor.AddComponent<HCapsuleColliderComponent>( "Unnamed Capsule Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == MeshColliderKey)
		{
			OutActor.AddComponent<HMeshColliderComponent>( "Unnamed Capsule Collider Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == LuaScriptKey)
		{
			OutActor.AddComponent<HLuaScriptComponent>( "Unnamed Lua Script Component" )
				->Deserialize( CurrentComponent );
		}
		else if (ComponentTypeHash == CameraBoomKey)
		{
			OutActor.AddComponent<HCameraBoomComponent>( "Unnamed Camera Boom Component" )
				->Deserialize( CurrentComponent );
		}
		else
		{
			HE_LOG( Error, TEXT( "Unrecognized component type when deserializing actor with name: %s" ), OutActor.GetObjectName() );
			HE_ASSERT( false );
		}
	}

	OutActor.OnDeserializeComplete();
}


/*static*/ void FActorSerializer::SerializeActor( AActor& Actor, JsonUtility::WriteContext& Output )
{
	if (Actor.m_IsDynamicInstance)
		return;

	Output.StartObject();
	{
		Output.Key( HE_STRINGIFY( AActor ) );
		Output.StartArray();
		{
			Output.StartObject();
			{
				Actor.Serialize( Output );
			}
			Output.EndObject();

			Output.StartObject();
			{
				Output.Key( "Components" );
				Output.StartArray();
				{
					for (uint32 i = 0; i < Actor.m_Components.size(); ++i)
					{
						Output.StartObject();
						{
							Output.Key( "ComponentType" );
							Output.String( Actor.m_Components[i]->GetComponenetStaticName() );
							Actor.m_Components[i]->Serialize( Output );
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