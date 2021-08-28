// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "GamePCH.h"
#include "MyGameInstance.h"


MyGameInstance::MyGameInstance()
{
	m_SomeVariable = 250;
	CreateLogger(GGameLogger, "Game");
}

MyGameInstance::~MyGameInstance()
{
}


HGameInstance* MakeGameInstance()
{
	return new MyGameInstance();
}
