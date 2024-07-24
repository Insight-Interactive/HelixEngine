#include "EnginePCH.h"

#include "GameFramework/Components/HFirstPersonCameraComponent.h"

#include "Engine/Engine.h"


HFirstPersonCameraComponent::HFirstPersonCameraComponent( FComponentInitArgs& InitArgs )
	: HCameraComponent( InitArgs )
	, m_CameraPitchSpeedMultiplier( kDefaultCameraPitchSpeedMultiplier )
	, m_CameraYawSpeedMultiplier( kDefaultCameraYawSpeedMultiplier )
	, m_CameraRollSpeedMultiplier( kDefaultCameraRollSpeedMultiplier )
{
}

HFirstPersonCameraComponent::~HFirstPersonCameraComponent()
{
}

void HFirstPersonCameraComponent::LookUp( float Value )
{
	const float kPitchRotationClamp = Math::DegreesToRadians( 88.f );

	m_Rotation.x += Value * m_CameraPitchSpeedMultiplier * GEngine->GetDeltaTime();
	m_Rotation.x = Math::Clamp( m_Rotation.x, -kPitchRotationClamp, kPitchRotationClamp );
	SetRotation( FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z ) );
}

void HFirstPersonCameraComponent::LookRight( float Value )
{
	m_Rotation.y += Value * m_CameraYawSpeedMultiplier * GEngine->GetDeltaTime();
	SetRotation( FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z ) );
}

void HFirstPersonCameraComponent::Roll( float Value )
{
	m_Rotation.z += Value * m_CameraRollSpeedMultiplier * GEngine->GetDeltaTime();
	SetRotation( FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z ) );
}
