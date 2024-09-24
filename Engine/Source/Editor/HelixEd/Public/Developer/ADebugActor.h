// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Actor/AActor.h"


class HFirstPersonCameraComponent;

class ADebugActor : public AActor
{
	using Super = AActor;
public:
	HE_GENERATED_BODY( ADebugActor );

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaMs ) override;

	HFirstPersonCameraComponent* GetCameraComponent();
	void SetCanMove( bool CanMove );

	void SetCanRotateCamera( bool CanRotate );

protected:
	void TogglePitchYawRotation();

	void UpdateMovement( float DeltaTime );
	void MoveManual( FVector3& Direction, float Value );
private:
	HFirstPersonCameraComponent* m_CameraComponent;
	bool m_CanRotateCamera;
	bool m_CanMove;

};


//
// Inline function implementations
//

inline HFirstPersonCameraComponent* ADebugActor::GetCameraComponent()
{
	return m_CameraComponent;
}

inline void ADebugActor::SetCanMove( bool CanMove )
{
	m_CanMove = CanMove;
}

inline void ADebugActor::SetCanRotateCamera( bool CanRotate )
{
	m_CanRotateCamera = CanRotate;
}
