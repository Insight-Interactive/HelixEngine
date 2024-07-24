// Copyright 2024 Insight Interactive. All Rights Reserved.
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

	inline HCameraComponent* GetCameraComponent();

protected:
	HCameraComponent* m_CameraComponent;
	HCapsuleColliderComponent* m_CharacterBounds;
};


//
// Inline function implementations
//

HCameraComponent* ACharacter::GetCameraComponent()
{
	return m_CameraComponent;
}
