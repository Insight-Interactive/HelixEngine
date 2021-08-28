// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Actor/APawn.h"

class HCameraComponent;

class APlayerCharacter : public APawn
{
public:
	APlayerCharacter( HWorld* pWorld );
	virtual ~APlayerCharacter();

	HE_GENERATED_BODY()

	virtual void BeginPlay() override
	{
		Super::BeginPlay();
	}

	virtual void Tick(float DeltaMs) override;

	virtual void SetupController(HControllerComponent& Controller) override;

	inline HCameraComponent* GetCameraComponent();

protected:
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
