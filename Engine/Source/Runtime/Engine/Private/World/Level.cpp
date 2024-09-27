// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/Level.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Actor/ACharacter.h"
#include "AssetRegistry/AssetDatabase.h"
#include "ThreadPool.h"
#include "AssetRegistry/ActorSerializer.h"


HLevel::HLevel( HWorld* pOwner )
	: HObject( "Level" )
	, m_pOwningWorld( pOwner )
{
}

HLevel::~HLevel()
{
	m_pOwningWorld = nullptr;
	Flush();
}

void HLevel::BeginPlay()
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->BeginPlay();
	}
}

void HLevel::Tick( float DeltaTime )
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Tick( DeltaTime );
	}
}

void HLevel::Flush()
{
	if (IsValid())
	{
		for (size_t i = 0; i < m_Actors.size(); ++i)
		{
			m_Actors[i]->RemoveAllComponents();
			HE_SAFE_DELETE_PTR( m_Actors[i] )
		}

		m_Actors.clear();
	}
}

void HLevel::Render( FCommandContext& CmdContext )
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Render( CmdContext );
	}
}

void HLevel::Serialize( JsonUtility::WriteContext& Output )
{
	for (auto Iter = m_Actors.begin(); Iter != m_Actors.end(); ++Iter)
	{
		AActor& CurrentActor = (**Iter);
		Output.Key( CurrentActor.GetGuid().ToString().CStr() );
		// Serialize the objects world transform as it exists in the level.
		Output.StartArray();
		{
			Output.StartObject();
			{
				FTransform& Transform = CurrentActor.GetTransform();
				// Position
				Output.Key( "PositionX" );
				Output.Double( Transform.GetPosition().x );
				Output.Key( "PositionY" );
				Output.Double( Transform.GetPosition().y );
				Output.Key( "PositionZ" );
				Output.Double( Transform.GetPosition().z );

				// Rotation
				Output.Key( "RotationX" );
				Output.Double( Transform.GetRotation().x );
				Output.Key( "RotationY" );
				Output.Double( Transform.GetRotation().y );
				Output.Key( "RotationZ" );
				Output.Double( Transform.GetRotation().z );

				// Scale
				Output.Key( "ScaleX" );
				Output.Double( Transform.GetScale().x );
				Output.Key( "ScaleY" );
				Output.Double( Transform.GetScale().y );
				Output.Key( "ScaleZ" );
				Output.Double( Transform.GetScale().z );
			}
			Output.EndObject();
		}
		Output.EndArray();
	}
}

void LoadActorAsyncMain( void* pData )
{

}

void HLevel::Deserialize( const JsonUtility::ReadContext& Value )
{
	m_IsLoading.Set();


	for (uint32 i = 0; i < Value.Size(); i++)
	{
		const rapidjson::Value& Actor = Value[i];
		for (auto Iter = Actor.MemberBegin(); Iter != Actor.MemberEnd(); Iter++)
		{
			FPath ActorPath;
			sprintf_s( ActorPath.m_Path, "%sActors\\%s", FGameProject::GetInstance()->GetContentFolder(), Iter->name.GetString() );

			// Load the actor
			//
			rapidjson::Document JsonDoc;
			FileRef JsonSource( ActorPath.GetFullPath(), FUM_Read );
			JsonUtility::LoadDocument( JsonSource, JsonDoc );
			if (JsonDoc.IsObject())
			{
				const Char* kBaseActorType = HE_STRINGIFY( AActor );
				//const Char* kPlayerCharacterType = HE_STRINGIFY( ACharacter );
				for (auto Itr = JsonDoc.MemberBegin(); Itr != JsonDoc.MemberEnd(); ++Itr)
				{
					const String ObjectType = Itr->name.GetString();
					if (ObjectType == kBaseActorType)
					{
						enum
						{
							kTransform = 0,
							kDataOverries = 1,
						};
						const char* ActorFileName = Iter->name.GetString();
						const rapidjson::Value& WorldOverrides = Actor[ActorFileName];
						const rapidjson::Value& ActorObject = JsonDoc[kBaseActorType];
						const rapidjson::Value& DataOverrides = WorldOverrides[kDataOverries];
						const rapidjson::Value& InstanceOverrides = DataOverrides["InstanceOverrides"];

						// Create the actor and deserialize its components.
						AActor* pNewActor = CreateActor<AActor>( "<Unnamed Actor>" );
						JsonUtility::GetTransform( WorldOverrides[kTransform], pNewActor->GetTransform() );
						FActorSerializer::DeserializeActor( *pNewActor, ActorObject );

						if (!InstanceOverrides.IsNull())
						{
							FActorSerializer::DeserializeActorOverrides( *pNewActor, InstanceOverrides );
						}
					}
				}
			}
			else
			{
				HE_LOG( Error, TEXT( "Failed to load actor with filepath: %s" ), CharToTChar( ActorPath.GetFullPath() ) );
				HE_ASSERT( false );
			}
		}
	}

	m_IsLoading.Clear();
}
