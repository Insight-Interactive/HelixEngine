// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameMacros.h"
#include "GameFwd.h"

#include "UI/Panel.h"
#include "UI/Label.h"

enum EMenuState
{
	MS_Invalid = 0,
	MS_Frontend,
	MS_InGame,
	MS_InGamePaused ,
	MS_MenuStateCount
};

class GAME_API InGameUI
{
	friend class MyGameInstance;
public:
	InGameUI();
	~InGameUI();

	void Tick( float DeltaTime );

private:

	void Setup();

	void OnGameSetFocus();
	void OnGameLostFocus();

	void OnMenuPressed();

	void UpdateWeaponInfoLabel();

private:
	EMenuState m_MenuState;


	FUIPanel m_InGameHUD;

	// Crosshairs
	FLabel m_CrosshairLabel;
	FLabel m_CurrentWeaponInfo;
	
	FUIPanel m_PauseMenuUI;
	FLabel m_PausedLabel;
};
