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

private:
	void UpdateCameraPitch();
	void UpdateCameraYaw();

private:
	bool m_UseCameraCollision;
	float m_CameraCollisionTraceDistance;
	float m_CameraDistance;
	FVector3 m_ViewOffset;

};
