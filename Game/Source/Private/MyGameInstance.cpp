// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "GamePCH.h"

#include "MyGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"
#include "GameFramework/Components/HSkeletalMeshComponenet.h"
#include "GameFramework/Components/HPointLightComponent.h"


extern HEngine* GEngine;

MyGameInstance::MyGameInstance()
{
	CreateLogger(GGameLogger, "Game");
}

MyGameInstance::~MyGameInstance()
{
}

void MyGameInstance::OnGameSetFocus()
{
	m_InGameMenu.OnGameSetFocus();
}

void MyGameInstance::OnGameLostFocus()
{
	m_InGameMenu.OnGameLostFocus();
}

void MyGameInstance::BeginPlay()
{
	Super::BeginPlay();

	AActor* Sun = GetWorld()->GetCurrentLevel().CreateActor<AActor>( TEXT( "Sun" ) );
	HPointLightComponent* Light = Sun->AddComponent<HPointLightComponent>( TEXT( "Sun" ) );
	Light->SetPosition( 0.f, 40.f, 100.f );
	Light->SetBrightness( 500.f );
	Light->SetRadius( 100.f );

	AActor* NewActor = GetWorld()->GetCurrentLevel().CreateActor<AActor>( TEXT( "Capsule Actor" ) );
	
	HSkeletalMeshComponent* pMesh = NewActor->AddComponent<HSkeletalMeshComponent>( TEXT( "SkeletalMesh" ) );
	pMesh->SetSkeletalMesh( FAssetDatabase::GetSkeletalMesh( "e1e1007c-f54e-4e84-9051-e6b7d0f02ff7" ) );//e1e1007c-f54e-4e84-9051-e6b7d0f02ff7 6123f7cc-4d94-47fb-ba9c-0455bae42877
	pMesh->SetRotation( Math::DegreesToRadians( 90.f ), 0.f, 0.f );

	/*FGUID MaterialGuid = FGUID::CreateFromString( "0d68e992-aa25-4aa4-9f81-0eb775320c1e" );
	pMesh->SetMaterial( FAssetDatabase::GetMaterial( MaterialGuid ) );
	HMaterial& Material = pMesh->GetMaterial();*/

	m_InGameMenu.Setup();
}

void MyGameInstance::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	

	m_InGameMenu.Tick( DeltaTime );
}

HGameInstance* MakeGameInstance()
{
	return new MyGameInstance();
}
