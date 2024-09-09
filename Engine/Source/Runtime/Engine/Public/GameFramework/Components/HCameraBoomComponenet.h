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

	virtual void Tick( float DeltaTime ) override;

	void SetCamera( HCameraComponent* Camera );
	void UpdateCameraPitch( float Value );
	void UpdateCameraYaw( float Value );
	void UseCameraCollision( bool UseCollision ) { m_UseCameraCollision = UseCollision; }
	void SetViewOffset( const FVector3& Offset );

	void SetBoomRotationAngles( FVector3& Angles ) 
	{ 
		m_Rotation = Angles; 
		UpdateCameraPitch( 0.f );
		UpdateCameraYaw( 0.f );
	}

private:
	bool m_UseCameraCollision;
	float m_CameraCollisionTraceDistance;
	HCameraComponent* m_Camera;

	FVector3 m_Rotation;

	FVector3 m_ViewOffset;

};
