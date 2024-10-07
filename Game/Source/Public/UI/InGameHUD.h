#pragma once

#include "GameMacros.h"
#include "GameFwd.h"

#include "UI/Panel.h"
#include "UI/Label.h"


class GAME_API InGameHUD
{
	friend class MyGameInstance;
public:
	InGameHUD();
	~InGameHUD();

private:
	void Setup();

	// Callbacks
	void UpdateWeaponInfoLabel();

private:

	FUIPanel m_InGameHUD;

	// Crosshairs
	FLabel m_CrosshairLabel;
	FLabel m_CurrentWeaponInfo;

};