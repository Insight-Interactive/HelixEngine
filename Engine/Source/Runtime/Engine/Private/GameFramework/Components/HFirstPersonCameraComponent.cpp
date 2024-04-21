#include "EnginePCH.h"

#include "GameFramework/Components/HFirstPersonCameraComponent.h"

#include "Engine/Engine.h"


HFirstPersonCameraComponent::HFirstPersonCameraComponent( FComponentInitArgs& InitArgs )
	: HCameraComponent( InitArgs )
	, m_CameraPitchSpeedMultiplier( kDefaultCameraPitchSpeedMultiplier )
	, m_CameraYawSpeedMultiplier( kDefaultCameraYawSpeedMultiplier )
{
}

HFirstPersonCameraComponent::~HFirstPersonCameraComponent()
{

}

void HFirstPersonCameraComponent::LookUp( float Value )
{
	const float kXRotationClamp = Math::DegreesToRadians( 88.f );

	m_Rotation.x += Value * m_CameraPitchSpeedMultiplier * (float)GEngine->GetDeltaTime();
	m_Rotation.x = Math::Clamp( m_Rotation.x, -kXRotationClamp, kXRotationClamp );
	m_RotX = FQuat::CreateFromAxisAngle( FVector3::Right, m_Rotation.x );
	SetRotation( m_RotX * m_RotY );
}

void HFirstPersonCameraComponent::LookRight( float Value )
{
	m_Rotation.y += Value * m_CameraYawSpeedMultiplier * (float)GEngine->GetDeltaTime();
	m_RotY = FQuat::CreateFromAxisAngle( FVector3::Up, m_Rotation.y );
	SetRotation( m_RotX * m_RotY );
}
