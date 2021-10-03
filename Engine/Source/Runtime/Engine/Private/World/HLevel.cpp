// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/HLevel.h"

#include "RendererCore.h"
#include "World/HWorld.h"
#include "FileSystem.h"
#include "StringHelper.h"
#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Actor/APlayerCharacter.h"
#include "AssetRegistry/AssetDatabase.h"

#include "JsonUtility.h"


HLevel::HLevel( HWorld* pOwner )
	: m_pOwningWorld( pOwner )
{
}

HLevel::~HLevel()
{
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
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->RemoveAllComponents();
		delete m_Actors[i];
	}
}

void HLevel::Render( ICommandContext& CmdContext )
{
	for (size_t i = 0; i < m_Actors.size(); ++i)
	{
		m_Actors[i]->Render( CmdContext );
	}
}

void HLevel::Serialize( WriteContext& Output )
{
	for (auto Iter = m_Actors.begin(); Iter != m_Actors.end(); ++Iter)
	{
		Output.Key( TCharToChar( (*Iter)->GetObjectName() ) );
		// Serialize the objects world transform as it exists in the level.
		Output.StartArray();
		{
			Output.StartObject();
			{
				Output.Key( "WorldTransform" );
				Output.StartArray();
				{
					Output.StartObject();
					{
						FTransform& ActorTransform = (*Iter)->GetTransform();
						// Position
						Output.Key( "PositionX" );
						Output.Double( ActorTransform.GetPosition().x );
						Output.Key( "PositionY" );
						Output.Double( ActorTransform.GetPosition().y );
						Output.Key( "PositionZ" );
						Output.Double( ActorTransform.GetPosition().z );

						// Rotation
						Output.Key( "RotationX" );
						Output.Double( ActorTransform.GetRotation().x );
						Output.Key( "RotationY" );
						Output.Double( ActorTransform.GetRotation().y );
						Output.Key( "RotationZ" );
						Output.Double( ActorTransform.GetRotation().z );

						// Scale
						Output.Key( "ScaleX" );
						Output.Double( ActorTransform.GetScale().x );
						Output.Key( "ScaleY" );
						Output.Double( ActorTransform.GetScale().y );
						Output.Key( "ScaleZ" );
						Output.Double( ActorTransform.GetScale().z );
					}
					Output.EndObject();
				}
				Output.EndArray();
			}
			Output.EndObject();
		}
		Output.EndArray();
	}
}

void HLevel::Deserialize( const ReadContext& Value )
{
	for (rapidjson::Value::ConstMemberIterator itr = Value.MemberBegin();
		itr != Value.MemberEnd(); ++itr)
	{
		String ActorName = itr->name.GetString();
		const rapidjson::Value& ActorWorldProps = itr->value;

		const String& ActorFilePath = AssetDatabase::GetInstance()->LookupActor( ActorName );
		HE_ASSERT( !ActorFilePath.empty() );

		// Load the actor
		//
		rapidjson::Document JsonDoc;
		FileRef JsonSource( ActorFilePath.c_str(), FUM_Read );
		JsonUtility::LoadDocument( JsonSource, JsonDoc );
		if (JsonDoc.IsObject())
		{
			const Char* kBaseActorType = HE_STRINGIFY( AActor );
			const Char* kPlayerCharacterType = HE_STRINGIFY( APlayerCharacter );
			for (rapidjson::Value::ConstMemberIterator Itr = JsonDoc.MemberBegin();
				Itr != JsonDoc.MemberEnd(); ++Itr)
			{
				String ObjectType = Itr->name.GetString();
				if (ObjectType == kBaseActorType)
				{
					const rapidjson::Value& ActorObject = JsonDoc[kBaseActorType];

					// Create the actor and deserialize its components.
					AActor* pNewActor = CreateActor<AActor>( TEXT( "<Unnamed Actor>" ) );
					pNewActor->Deserialize( ActorObject );
					JsonUtility::GetTransform( ActorWorldProps[0], "WorldTransform", pNewActor->GetTransform() );
				}
				else if (ObjectType == kPlayerCharacterType)
				{
					APlayerCharacter* pPlayer = CreateActor<APlayerCharacter>( TEXT( "<Unnamed Player Character>" ) );
					GetWorld()->SetCurrentSceneRenderCamera( pPlayer->GetCameraComponent() );
					GetWorld()->AddPlayerCharacterRef( pPlayer );
				}
			}
		}
	}
}
