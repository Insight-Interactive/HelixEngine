// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/ACharacter.h"


class HCameraBoomComponent;

class AThirdPersonCharacter : public ACharacter
{
	using Super = ACharacter;
public:
	HE_GENERATED_BODY( AThirdPersonCharacter );

	virtual void Tick( float DeltaTime ) override;

private:
	virtual void SetupController( HControllerComponent& Controller ) override;

	void LookUp( float Value );
	void LookRight( float Value );
	void ThirdPersonMoveForward( float Delta );
	void ThirdPersonMoveRight( float Delta );

private:
	HCameraBoomComponent* m_pCameraBoom;

};
