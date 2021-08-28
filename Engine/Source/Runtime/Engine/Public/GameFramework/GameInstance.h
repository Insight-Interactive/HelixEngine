// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"
#include "TSingleton.h"

#if GAME_MODULE
#	define GAME_API __declspec(dllexport)
#else
#	define GAME_API __declspec(dllimport)
#endif

class GAME_API HGameInstance
{
public:
	HGameInstance()
	{
	}
	virtual ~HGameInstance()
	{
	}

protected:
	uint32 m_SomeVariable;

};

static HGameInstance* GGameInstance = NULL;

GAME_API HGameInstance* MakeGameInstance();
