// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "HActorComponent.h"


class HCameraComponent;

HCOMPONENT()
class HCameraBoomComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HCameraBoomComponent );

	virtual void Tick( float DeltaTime ) override;

	void SetCamera( HCameraComponent* Camera );
	void UpdateCameraPitch( float Value );
	void UpdateCameraYaw( float Value );
	void UseCameraCollision( bool UseCollision ) { m_UseCameraCollision = UseCollision; }
	void SetViewOffset( const FVector3& Offset );

	void SetBoomRotationAngles( FAngles& Angles ) 
	{ 
		m_Rotation = Angles; 
		UpdateCameraPitch( 0.f );
		UpdateCameraYaw( 0.f );
	}

	FTransform& GetTransform() { return m_Transform; }

private:
	FTransform m_Transform;

	bool m_UseCameraCollision;
	float m_CameraPitchSpeed;
	float m_CameraYawSpeed;
	float m_CameraCollisionTraceDistance;
	HCameraComponent* m_Camera;

	FAngles m_Rotation;

	FVector3 m_ViewOffset;

};
