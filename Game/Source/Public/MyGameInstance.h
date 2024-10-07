// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CoreFwd.h"
#include "GameMacros.h"
#include "GameFwd.h"

#include "GameFramework/GameInstance.h"
#include "UI/InGameUI.h"


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
	InGameUI m_InGameMenu;

};
