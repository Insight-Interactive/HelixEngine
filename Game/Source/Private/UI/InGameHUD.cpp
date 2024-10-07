#include "GamePCH.h"

#include "UI/InGameHUD.h"

#include "GameFramework/GameInstance.h"
#include "World/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor/AThirdPersonCharacter.h"


InGameHUD::InGameHUD()
{
}

InGameHUD::~InGameHUD()
{
}

void InGameHUD::Setup()
{
	// Init the ui
	m_InGameHUD.Show();
	HGameInstance::GetInstance()->GetWorld()->AddPanel( &m_InGameHUD );

	m_CurrentWeaponInfo.SetText( L"- / -" );
	m_CurrentWeaponInfo.SetPosition( { 0.85f, 0.85f } );
	m_CrosshairLabel.SetText( L"." );
	m_CrosshairLabel.SetPosition( { 0.5f, 0.47f } ); // TODO: This shouls be an image
	m_InGameHUD.AddWidget( m_CurrentWeaponInfo );
	m_InGameHUD.AddWidget( m_CrosshairLabel );

	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback("FireWeapon", IE_Pressed, std::bind( &InGameHUD::UpdateWeaponInfoLabel, this));
	GEngine->GetClientViewport().GetInputDispatcher()->RegisterActionCallback("ReloadWeapon", IE_Pressed, std::bind( &InGameHUD::UpdateWeaponInfoLabel, this));
	
	UpdateWeaponInfoLabel();
}

void InGameHUD::UpdateWeaponInfoLabel()
{
	FWeapon* Weapon = GEngine->GetGameWorld().GetPlayerCharacter()->GetCurrentWeapon();
	m_CurrentWeaponInfo.SetText( L"%i / %i", (int)Weapon->m_MagazineAmmo, (int)Weapon->m_ReserveAmmo );
}
