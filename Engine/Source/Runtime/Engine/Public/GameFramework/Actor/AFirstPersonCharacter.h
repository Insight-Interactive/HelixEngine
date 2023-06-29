#pragma once


#include "GameFramework/Actor/ACharacter.h"

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

private:
	FVector2 m_Rotation;
	FQuat m_RotX;
	FQuat m_RotY;
	bool m_CanRotateCamera;

};
