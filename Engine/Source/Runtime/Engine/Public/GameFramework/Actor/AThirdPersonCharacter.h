// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/ACharacter.h"

#include "GameFramework/Ballistics/Weapon.h"


class HCameraBoomComponent;
class HStaticMeshComponent;

HCLASS()
class AThirdPersonCharacter : public ACharacter
{
	using Super = ACharacter;
public:
	HE_GENERATED_BODY( AThirdPersonCharacter );

	virtual void Tick( float DeltaTime ) override;
	inline HCameraComponent* GetCameraComponent();

	FWeapon* GetCurrentWeapon() { return m_CurrentWeapon; }

private:
	FVector3 GetShootDirection();

	virtual void SetupController( HControllerComponent& Controller ) override;

	// Input Callbacks
	void ThirdPersonMoveForward( float Delta );
	void ThirdPersonMoveRight( float Delta );
	void ThirdPersonSprint();
	void AimDownSight();
	void FireWeaponPressed();
	void FireWeaponHeld();
	void FireWeaponReleased();
	void ReloadWeapon();
	void DoMelee();


private:
	HCameraComponent*		m_CameraComponent;
	HCameraBoomComponent*	m_CameraBoom;

	// Weapons
	FWeapon* m_CurrentWeapon;
	FSemiAutomaticWeapon m_Rifle;

	float m_ADSFOVDegrees;
	float m_CameraFOV;
	bool m_IsAiming;
	float m_ADSTimeSeconds;

};


//
// Inline function implementations
//

HCameraComponent* AThirdPersonCharacter::GetCameraComponent()
{
	return m_CameraComponent;
}
