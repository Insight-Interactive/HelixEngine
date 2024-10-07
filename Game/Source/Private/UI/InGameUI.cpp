#include "GamePCH.h"

#include "UI/InGameUI.h"

#include "Engine/Engine.h"
#include "GameFramework/GameInstance.h"
#include "Engine/ViewportContext.h"
#include "Input/InputDispatcher.h"
#include "GameFramework/Actor/AThirdPersonCharacter.h"


InGameUI::InGameUI()
	: m_MenuState( MS_InGame )
{
}

InGameUI::~InGameUI()
{
}

void InGameUI::Setup()
{
	// Init the ui
	m_InGameHUD.Show();
	HGameInstance::GetInstance()->GetWorld()->AddPanel( &m_InGameHUD );
	
	m_CurrentWeaponInfo.SetText( L"-" );
	m_CurrentWeaponInfo.SetPosition( { 0.85f, 0.85f } );
	m_CrosshairLabel.SetText( L"." );
	m_CrosshairLabel.SetPosition( { 0.5f, 0.47f } ); // TODO: This shouls be an image
	m_InGameHUD.AddWidget( m_CurrentWeaponInfo );
	m_InGameHUD.AddWidget( m_CrosshairLabel );

	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback( "FireWeapon", IE_Pressed, std::bind( &InGameUI::UpdateWeaponInfoLabel, this ) );
	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback( "ReloadWeapon", IE_Pressed, std::bind( &InGameUI::UpdateWeaponInfoLabel, this ) );

	UpdateWeaponInfoLabel();

	m_PausedLabel.SetText( L"Paused" );
	m_PausedLabel.SetPosition( { 0.f, 0.2f } );
	m_PauseMenuUI.AddWidget( m_PausedLabel );
	m_PauseMenuUI.Hide();
	HGameInstance::GetInstance()->GetWorld()->AddPanel( &m_PauseMenuUI );


	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback( "Menu", IE_Pressed, std::bind( &InGameUI::OnMenuPressed, this ) );
}

void InGameUI::UpdateWeaponInfoLabel()
{
	FWeapon* Weapon = GEngine->GetGameWorld().GetPlayerCharacter()->GetCurrentWeapon();
	if (Weapon->GetStaticType() == WT_BulletWeapon)
	{
		FBulletWeapon* BulletWeapon = (FBulletWeapon*)Weapon;
		m_CurrentWeaponInfo.SetText( L"%i / %i", BulletWeapon->m_MagazineAmmo, BulletWeapon->m_ReserveAmmo );
	}
}

void InGameUI::OnMenuPressed()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	static bool Paused = false;
	Paused = !Paused;
	if (Paused)
	{
		GEngine->TogglePauseGame( true );

		m_InGameHUD.Hide();
		m_PauseMenuUI.Show();
		Input::UnacquireMouse();
	}
	else
	{
		GEngine->TogglePauseGame( false );

		m_InGameHUD.Show();
		m_PauseMenuUI.Hide();
		Input::AcquireMouse();
	}
}

void InGameUI::Tick( float DeltaTime )
{
}

void InGameUI::OnGameSetFocus()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGamePaused:
		m_InGameHUD.Hide();
		m_PauseMenuUI.Show();
		Input::UnacquireMouse();
		break;
	case MS_InGame:
		m_InGameHUD.Show();
		m_PauseMenuUI.Hide();
		Input::AcquireMouse();
		break;
	}
}

void InGameUI::OnGameLostFocus()
{
	FViewportContext& ClientViewport = GEngine->GetClientViewport();

	switch (m_MenuState)
	{
	case MS_Frontend:
		break;
	case MS_InGamePaused:
		m_InGameHUD.Hide();
		m_PauseMenuUI.Show();
		break;
	case MS_InGame:
		Input::UnacquireMouse();
		break;
	}
}
