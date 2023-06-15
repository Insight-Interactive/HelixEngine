// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "GamePCH.h"

#include "MyGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/Actor/AActor.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


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
HCapsuleColliderComponent* Comp;
void MyGameInstance::BeginPlay()
{
	Super::BeginPlay();

	AActor* NewActor = GetWorld()->GetCurrentLevel().CreateActor<AActor>( TEXT( "Capsule Actor" ) );
	Comp = NewActor->AddComponent<HCapsuleColliderComponent>( TEXT( "Bounds" ) );
	Comp->SetPosition( 0.f, 50.f, 0.f );

	m_InGameMenu.Setup();
}

void MyGameInstance::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	

	//Comp->Rotate( 0.f, 0.f, 0.01 );

	m_InGameMenu.Tick( DeltaTime );
}

HGameInstance* MakeGameInstance()
{
	return new MyGameInstance();
}
