#pragma once

#include "GameFramework/Actor/APawn.h"


class HCameraComponent;

class ADebugPawn : public APawn
{
	using Super = APawn;
public:
	HE_GENERATED_BODY( ADebugPawn );

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaMs ) override;

	HCameraComponent* GetCameraComponent();
	void SetCanMove( bool CanMove );

protected:
	void LookUp( float Value );
	void LookRight( float Value );
	void TogglePitchYawRotation();

	void UpdateMovement( float DeltaTime );

private:
	HCameraComponent* m_pCameraComponent;
	bool m_CanRotateCamera;
	bool m_CanMove;

};


//
// Inline function implementations
//

inline HCameraComponent* ADebugPawn::GetCameraComponent()
{
	return m_pCameraComponent;
}

inline void ADebugPawn::SetCanMove( bool CanMove )
{
	m_CanMove = CanMove;
}
