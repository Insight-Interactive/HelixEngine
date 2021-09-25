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

void HLevel::LoadFromFile( const Char* FileName )
{
	APlayerCharacter* pPlayer = CreateActor<APlayerCharacter>( TEXT( "Player Character" ) );
	GetWorld()->SetCurrentSceneRenderCamera( pPlayer->GetCameraComponent() );
	GetWorld()->AddPlayerCharacterRef( pPlayer );

	rapidjson::Document LevelJsonDoc;
	FileRef LevelJsonSource( FileName, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( LevelJsonSource, LevelJsonDoc );
	if (LevelJsonDoc.IsObject())
	{
		for (rapidjson::Value::ConstMemberIterator itr = LevelJsonDoc.MemberBegin();
			itr != LevelJsonDoc.MemberEnd(); ++itr)
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
				const Char* kBaseActorType = "BaseActor";
				const Char* kPlayerCharacterType = "PlayerCharacter";
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

	HE_LOG( Log, TEXT( "Level loaded with name: " ), StringHelper::GetFilenameFromDirectoryNoExtension( FileName ) );
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
