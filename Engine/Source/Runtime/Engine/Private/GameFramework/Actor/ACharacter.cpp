// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/ACharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HSkeletalMeshComponent.h"
#include "AssetRegistry/AssetDatabase.h"


ACharacter::ACharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_Body = AddComponent<HSkeletalMeshComponent>( "Body" );
	m_Body->GetTransform().LinkTo( m_Transform );

	// Load relaxed animations
	m_IdleRelaxed = FAssetDatabase::GetAnimation( "BipedalLocamotion/Idle_Relaxed.fbx" );
	m_WalkRelaxed = FAssetDatabase::GetAnimation( "BipedalLocamotion/Walk_Relaxed.fbx" );
	m_IdleRelaxedToWalkRelaxed = FAssetDatabase::GetAnimation( "BipedalLocamotion/Idle_Relaxed_To_Walk_Relaxed.fbx" );
	m_WalkRelaxedToIdleRelaxed = FAssetDatabase::GetAnimation( "BipedalLocamotion/Walk_Relaxed_To_Idle_Relaxed.fbx" );
	
	m_Body->SetAnimation( m_IdleRelaxed );
}

ACharacter::~ACharacter()
{

}

