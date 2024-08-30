#include "GamePCH.h"

#include "GameMenu.h"

#include "Engine/Engine.h"
#include "GameFramework/GameInstance.h"
#include "Engine/ViewportContext.h"
#include "Input/InputDispatcher.h"


GameMenu::GameMenu()
	: m_MenuState( MS_InGame )
{
}

GameMenu::~GameMenu()
{

}

void GameMenu::Setup()
{
	// Init the ui
	m_InGameUI.Show();
	HGameInstance::GetInstance()->GetWorld()->AddPanel( &m_InGameUI );
	
	m_PausedLabel.SetText( L"Paused" );
	m_PausedLabel.SetPosition( { 0.f, 0.2f } );
	m_PauseMenuUI.AddWidget( m_PausedLabel );
	m_PauseMenuUI.Hide();
	HGameInstance::GetInstance()->GetWorld()->AddPanel( &m_PauseMenuUI );


	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback( "Menu", IE_Pressed, std::bind( &GameMenu::OnMenuPressed, this ) );
}

void GameMenu::OnMenuPressed()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	static bool Paused = false;
	Paused = !Paused;
	if (Paused)
	{
		GEngine->TogglePauseGame( true );

		m_InGameUI.Hide();
		m_PauseMenuUI.Show();
		Input::ShowMouse();
		Input::UnacquireMouse();
	}
	else
	{
		GEngine->TogglePauseGame( false );

		m_InGameUI.Show();
		m_PauseMenuUI.Hide();
		Input::HideMouse();
		Input::AcquireMouse();
	}
}

void GameMenu::Tick( float DeltaTime )
{
}

void GameMenu::OnGameSetFocus()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGamePaused:
		m_InGameUI.Hide();
		m_PauseMenuUI.Show();
		Input::ShowMouse();
		Input::UnacquireMouse();
		break;
	case MS_InGame:
		m_InGameUI.Show();
		m_PauseMenuUI.Hide();
		Input::HideMouse();
		Input::AcquireMouse();
		break;
	}
}

void GameMenu::OnGameLostFocus()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGamePaused:
		m_InGameUI.Hide();
		m_PauseMenuUI.Show();
		break;
	case MS_InGame:
		Input::ShowMouse();
		Input::UnacquireMouse();
		break;
	}
}
