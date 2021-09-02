// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "TSingleton.h"

#if GAME_MODULE
#	define GAME_API __declspec( dllexport )
#else
#	define GAME_API __declspec( dllimport )
#endif

class APlayerCharacter;

class GAME_API HGameInstance : public TSingleton<HGameInstance>
{
public:
	HGameInstance() {}
	virtual ~HGameInstance() {}

	virtual void OnGameSetFocus() {}
	virtual void OnGameLostFocus() {}


protected:

};

static HGameInstance* GGameInstance = NULL;

GAME_API HGameInstance* MakeGameInstance();

