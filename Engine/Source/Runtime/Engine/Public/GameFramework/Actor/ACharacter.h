// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/APawn.h"


class HCameraComponent;
class HCapsuleColliderComponent;

HCLASS()
class ACharacter : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( ACharacter );

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaMs) override;

	inline HCameraComponent* GetCameraComponent();

protected:
	HCameraComponent* m_pCameraComponent;
	HCapsuleColliderComponent* m_pCharacterBounds;

};


//
// Inline function implementations
//

HCameraComponent* ACharacter::GetCameraComponent()
{
	return m_pCameraComponent;
}
