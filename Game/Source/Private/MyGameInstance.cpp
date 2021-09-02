// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "GamePCH.h"
#include "MyGameInstance.h"


MyGameInstance::MyGameInstance()
{
	CreateLogger(GGameLogger, "Game");
}

MyGameInstance::~MyGameInstance()
{
}


HGameInstance* MakeGameInstance()
{
	return new MyGameInstance();
}
