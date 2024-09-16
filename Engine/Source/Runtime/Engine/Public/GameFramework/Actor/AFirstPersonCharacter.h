// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


#include "GameFramework/Actor/ACharacter.h"

HCLASS()
class AFirstPersonCharacter : public ACharacter
{
	using Super = ACharacter;
public:
	HE_GENERATED_BODY( AFirstPersonCharacter );

private:
	virtual void SetupController( HControllerComponent& Controller ) override;

	void FirstPersonMoveForward( float Value );
	void FirstPersonMoveRight( float Value );
	void LookUp( float Value );
	void LookRight( float Value );

	void TogglePitchYawRotation();
	inline HCameraComponent* GetCameraComponent();

private:
	HCameraComponent* m_CameraComponent;

	bool m_CanRotateCamera;

};


//
// Inline function implementations
//

HCameraComponent* AFirstPersonCharacter::GetCameraComponent()
{
	return m_CameraComponent;
}
