#pragma once

#include "GameMacros.h"
#include "GameFwd.h"

enum EMenuState
{
	MS_Invalid = 0,
	MS_Frontend,
	MS_InGame,
	MS_InGamePaused ,
	MS_MenuStateCount
};

class GAME_API GameMenu
{
	friend class MyGameInstance;
public:
	GameMenu();
	~GameMenu();

private:
	void OnGameSetFocus();
	void OnGameLostFocus();

private:
	EMenuState m_MenuState;
};
