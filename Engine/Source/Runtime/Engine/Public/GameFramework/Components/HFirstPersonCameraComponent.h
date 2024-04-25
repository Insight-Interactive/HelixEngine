#pragma once

#include "GameFramework/Components/HCameraComponent.h"


static const float kDefaultCameraPitchSpeedMultiplier = 50.f;
static const float kDefaultCameraYawSpeedMultiplier = 50.f;
static const float kDefaultCameraRollSpeedMultiplier = 50.f;

class HFirstPersonCameraComponent : public HCameraComponent
{
	friend class HCameraManager;
public:
	HE_COMPONENT_GENERATED_BODY( HFirstPersonCameraComponent );

	void LookUp( float Value );
	void LookRight( float Value );
	void Roll( float Value );

	float GetVerticalLookSpeed() const;
	float GetHorizontalLookSpeed() const;
	void SetVerticalLookSpeed( float Speed );
	void SetHorizontalLookSpeed( float Speed );

	FVector3 m_Rotation;
private:
	FQuat m_RotX;
	FQuat m_RotY;
	FQuat m_RotZ;

	float m_CameraPitchSpeedMultiplier;
	float m_CameraYawSpeedMultiplier;
	float m_CameraRollSpeedMultiplier;

};

//
// Inline function implementations
//


inline float HFirstPersonCameraComponent::GetVerticalLookSpeed() const
{
	return m_CameraPitchSpeedMultiplier;
}

inline float HFirstPersonCameraComponent::GetHorizontalLookSpeed() const
{
	return m_CameraYawSpeedMultiplier;
}

inline void HFirstPersonCameraComponent::SetVerticalLookSpeed( float Speed )
{
	m_CameraPitchSpeedMultiplier = Speed;
}

inline void HFirstPersonCameraComponent::SetHorizontalLookSpeed( float Speed )
{
	m_CameraYawSpeedMultiplier = Speed;
}