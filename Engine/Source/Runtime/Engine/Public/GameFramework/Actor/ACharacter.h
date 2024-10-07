// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/APawn.h"


class HCameraComponent;
class HCapsuleColliderComponent;
class HSkeletalMeshComponent;

HCLASS()
class ACharacter : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( ACharacter );



protected:
	HSkeletalMeshComponent* m_Body;

	// Relaxed Animations
	HAnimation m_IdleRelaxed;
	HAnimation m_WalkRelaxed;
	HAnimation m_RunRelaxed;
	HAnimation m_IdleRelaxedToWalkRelaxed;
	HAnimation m_WalkRelaxedToIdleRelaxed;
};
