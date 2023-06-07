// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "GamePCH.h"

#include "MyGameInstance.h"

#include "Engine/Engine.h"


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

HGameInstance* MakeGameInstance()
{
	return new MyGameInstance();
}
