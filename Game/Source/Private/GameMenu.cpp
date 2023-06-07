#include "GamePCH.h"

#include "GameMenu.h"

#include "Engine/Engine.h"


GameMenu::GameMenu()
	: m_MenuState( MS_InGame )
{

}

GameMenu::~GameMenu()
{

}

void GameMenu::OnGameSetFocus()
{
	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGame:
		GEngine->GetClientViewport().HideMouse();
		GEngine->GetClientViewport().LockMouseToScreenCenter();
		break;
	}
}

void GameMenu::OnGameLostFocus()
{
	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGame:
		GEngine->GetClientViewport().ShowMouse();
		GEngine->GetClientViewport().UnlockMouseFromScreenCenter();
		break;
	}
}
