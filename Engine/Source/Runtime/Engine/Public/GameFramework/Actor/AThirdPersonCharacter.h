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

private:
	virtual void SetupController( HControllerComponent& Controller ) override;

	void ThirdPersonMoveForward( float Delta );
	void ThirdPersonMoveRight( float Delta );
	void AimDownSight();
	void FireWeapon();
	void DoMelee();

private:
	HCameraBoomComponent* m_CameraBoom;
	HStaticMeshComponent* m_Body;

	float m_ADSFOVDegrees;
	float m_CameraFOV;
	bool m_IsAiming;
	float m_ADSTimeSeconds;

};
