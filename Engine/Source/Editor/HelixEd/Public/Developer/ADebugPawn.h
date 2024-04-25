#pragma once

#include "GameFramework/Actor/APawn.h"


class HFirstPersonCameraComponent;

class ADebugPawn : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( ADebugPawn );

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaMs ) override;

	HFirstPersonCameraComponent* GetCameraComponent();
	void SetCanMove( bool CanMove );

	void SetCanRotateCamera( bool CanRotate );

protected:
	void TogglePitchYawRotation();

	void UpdateMovement( float DeltaTime );

private:
	HFirstPersonCameraComponent* m_pCameraComponent;
	bool m_CanRotateCamera;
	bool m_CanMove;

};


//
// Inline function implementations
//

inline HFirstPersonCameraComponent* ADebugPawn::GetCameraComponent()
{
	return m_pCameraComponent;
}

inline void ADebugPawn::SetCanMove( bool CanMove )
{
	m_CanMove = CanMove;
}

inline void ADebugPawn::SetCanRotateCamera( bool CanRotate )
{
	m_CanRotateCamera = CanRotate;
}
