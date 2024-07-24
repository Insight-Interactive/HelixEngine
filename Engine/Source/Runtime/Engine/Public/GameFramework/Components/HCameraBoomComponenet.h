// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "HSceneComponent.h"


class HCameraComponent;

HCOMPONENT()
class HCameraBoomComponent : public HSceneComponent
{
	friend class AActor;
	using Super = HSceneComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HCameraBoomComponent );

	void Update( float DeltaTime );

	void SetCamera( HCameraComponent* Camera );
	void UpdateCameraPitch( float Value );
	void UpdateCameraYaw( float Value );
	void UseCameraCollision( bool UseCollision ) { m_UseCameraCollision = UseCollision; }
	void SetViewOffset( const FVector3& Offset ) { m_ViewOffset = Offset; }

private:
	bool m_UseCameraCollision;
	float m_CameraCollisionTraceDistance;
	float m_CameraDistance;
	FVector3 m_ViewOffset;
	HCameraComponent* m_Camera;

	FVector3 m_Rotation;

};
