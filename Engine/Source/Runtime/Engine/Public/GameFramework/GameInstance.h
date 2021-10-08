// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "TSingleton.h"

#include "Engine/Engine.h"

#if GAME_MODULE
#	define GAME_API __declspec( dllexport )
#else
#	define GAME_API __declspec( dllimport )
#endif

class APlayerCharacter;

class /*GAME_API*/ HGameInstance : public TSingleton<HGameInstance>
{
public:
	HGameInstance() {}
	virtual ~HGameInstance() {}

	virtual void OnGameSetFocus() 
	{
		GEngine->GetClientViewport().LockMouseToScreenCenter();
	}
	virtual void OnGameLostFocus() 
	{
		GEngine->GetClientViewport().UnlockMouseFromScreenCenter();
	}

protected:

};

/*static*/ extern HGameInstance* GGameInstance;

GAME_API HGameInstance* MakeGameInstance();
