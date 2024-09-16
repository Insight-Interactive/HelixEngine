// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/ACharacter.h"


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

private:
	virtual void SetupController( HControllerComponent& Controller ) override;

	void ThirdPersonMoveForward( float Delta );
	void ThirdPersonMoveRight( float Delta );
	void AimDownSight();
	void FireWeapon();
	void DoMelee();

private:
	HCameraComponent*		m_CameraComponent;
	HCameraBoomComponent*	m_CameraBoom;

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