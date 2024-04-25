// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"
#include "GameMacros.h"
#include "GameFwd.h"

#include "GameFramework/GameInstance.h"
#include "GameMenu.h"

class GAME_API MyGameInstance : public HGameInstance
{
	using Super = HGameInstance;
public:
	MyGameInstance();
	virtual ~MyGameInstance();

	virtual void OnGameSetFocus() override;
	virtual void OnGameLostFocus() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

private:
	GameMenu m_InGameMenu;

};
