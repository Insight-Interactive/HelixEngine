// Copyright 2024 Insight Interactive. All Rights Reserved.
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

void MyGameInstance::BeginPlay()
{
	Super::BeginPlay();

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
