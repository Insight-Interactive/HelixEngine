// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/APawn.h"

class HCameraComponent;

HCLASS()
class APlayerCharacter : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( APlayerCharacter );

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaMs) override;

	virtual void SetupController(HControllerComponent& Controller) override;

	inline HCameraComponent* GetCameraComponent();

	void LookUp(float Value);
	void LookRight(float Value);
	void TogglePitchYawRotation();

protected:
	HCameraComponent* m_pCameraComponent;

private:
	bool m_CanRotateCamera;
};


//
// Inline function implementations
//

HCameraComponent* APlayerCharacter::GetCameraComponent()
{
	return m_pCameraComponent;
}
