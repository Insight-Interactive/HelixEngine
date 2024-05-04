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

	void UpdateCameraPitch( float Value );
	void UpdateCameraYaw( float Value );

private:
	bool m_UseCameraCollision;
	float m_CameraCollisionTraceDistance;
	float m_CameraDistance;
	FVector3 m_ViewOffset;

};
