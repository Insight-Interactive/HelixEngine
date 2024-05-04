// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/ACharacter.h"


class HCameraBoomComponent;
class HSkeletalMeshComponent;

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

private:
	HCameraBoomComponent* m_pCameraBoom;
	HSkeletalMeshComponent* m_pCharacterMesh;

	float m_ADSFOVDegrees;
	float m_CameraFOV;
	bool m_IsAiming;
	float m_ADSTimeSeconds;

};
