// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/APawn.h"


class HCameraComponent;
class HCapsuleColliderComponent;
class HStaticMeshComponent;

HCLASS()
class ACharacter : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( ACharacter );


protected:
	HStaticMeshComponent* m_Body;

};
