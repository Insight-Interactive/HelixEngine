// Copyright 2024 Insight Interactive. All Rights Reserved.
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
	FVector3 GetCameraAngles();
	void SetCameraAngles( FVector3& Angles );

private:
	FVector3 m_Rotation;

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

inline FVector3 HFirstPersonCameraComponent::GetCameraAngles()
{
	return m_Rotation;
}

inline void HFirstPersonCameraComponent::SetCameraAngles( FVector3& Angles )
{
	m_Rotation = Angles;
	m_Transform.SetRotation( FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z ) );
}
