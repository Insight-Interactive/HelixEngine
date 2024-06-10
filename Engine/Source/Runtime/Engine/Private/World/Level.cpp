// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/Level.h"

#include "World/World.h"
#include "RendererCore.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Actor/ACharacter.h"
#include "AssetRegistry/AssetDatabase.h"
#include "ThreadPool.h"


HLevel::HLevel( HWorld* pOwner )
	: m_pOwningWorld( pOwner )
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
				if (HSceneComponent* pRoot = CurrentActor.GetRootComponent())
				{
					// Position
					Output.Key( "PositionX" );
					Output.Double( pRoot->GetPosition().x );
					Output.Key( "PositionY" );
					Output.Double( pRoot->GetPosition().y );
					Output.Key( "PositionZ" );
					Output.Double( pRoot->GetPosition().z );

					// Rotation
					Output.Key( "RotationX" );
					Output.Double( pRoot->GetRotation().x );
					Output.Key( "RotationY" );
					Output.Double( pRoot->GetRotation().y );
					Output.Key( "RotationZ" );
					Output.Double( pRoot->GetRotation().z );

					// Scale
					Output.Key( "ScaleX" );
					Output.Double( pRoot->GetScale().x );
					Output.Key( "ScaleY" );
					Output.Double( pRoot->GetScale().y );
					Output.Key( "ScaleZ" );
					Output.Double( pRoot->GetScale().z );
				}

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
	for (auto Iter = Value.MemberBegin(); Iter != Value.MemberEnd(); Iter++)
	{
		char ActorPath[HE_MAX_PATH];
		sprintf_s( ActorPath, "%sActors\\%s", FGameProject::GetInstance()->GetContentFolder(), Iter->name.GetString() );

		// Load the actor
		//
		rapidjson::Document JsonDoc;
		FileRef JsonSource( ActorPath, FUM_Read );
		JsonUtility::LoadDocument( JsonSource, JsonDoc );
		if (JsonDoc.IsObject())
		{
			const Char* kBaseActorType = HE_STRINGIFY( AActor );
			const Char* kPlayerCharacterType = HE_STRINGIFY( ACharacter );
			for (auto Itr = JsonDoc.MemberBegin(); Itr != JsonDoc.MemberEnd(); ++Itr)
			{
				const String ObjectType = Itr->name.GetString();
				if (ObjectType == kBaseActorType)
				{
					const rapidjson::Value& ActorObject = JsonDoc[kBaseActorType];

					// Create the actor and deserialize its components.
					AActor* pNewActor = CreateActor<AActor>( TEXT( "<Unnamed Actor>" ) );
					pNewActor->Deserialize( ActorObject );
					if (HSceneComponent* pRoot = pNewActor->GetRootComponent())
					{
						FTransform Transform;
						JsonUtility::GetTransform( Value, Iter->name.GetString(), Transform );
						FVector3 Pos = Transform.GetPosition();
						pRoot->Translate( Pos.x, Pos.y, Pos.z );
						FVector3 Rot = Transform.GetRotation().ToEulerAngles();
						pRoot->Rotate( Rot.x, Rot.y, Rot.z );
						FVector3 Sca = Transform.GetScale();
						pRoot->Scale( Sca.x, Sca.y, Sca.z );
					}
					pNewActor->OnDeserializeComplete();
				}
			}
		}
		else
		{
			HE_LOG( Error, TEXT( "Failed to load actor with filepath: %s" ), CharToTChar( ActorPath ) );
			HE_ASSERT( false );
		}
	}
	m_IsLoading.Clear();
}
