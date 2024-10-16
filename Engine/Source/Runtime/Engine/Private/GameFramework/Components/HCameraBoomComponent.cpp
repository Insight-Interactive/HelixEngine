#include "EnginePCH.h"

#include "GameFramework/Components/HCameraBoomComponenet.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"


HCameraBoomComponent::HCameraBoomComponent( FComponentInitArgs& InitArgs )
	: HActorComponent( InitArgs )
	, m_UseCameraCollision( true )
	, m_CameraCollisionTraceDistance( 5.f )
	, m_CameraPitchSpeed( 60.f )
	, m_CameraYawSpeed( 60.f )
{
}

HCameraBoomComponent::~HCameraBoomComponent()
{

}

void HCameraBoomComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	if( m_UseCameraCollision )
	{
		/*if (Physics::SphereCast( m_Camera->GetTransform().GetWorldPosition(), m_CameraCollisionTraceDistance ))
			HE_LOG( Log, "Camera Hit!" );*/
	}
}

void HCameraBoomComponent::UpdateCameraPitch( float PitchDelta )
{
	const float kPitchRotationClamp = Math::DegreesToRadians( 50.f );
	m_Rotation.pitch += -(PitchDelta * m_CameraPitchSpeed) * GEngine->GetDeltaTime();
	m_Rotation.pitch = Math::Clamp( m_Rotation.pitch, -kPitchRotationClamp, kPitchRotationClamp );
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.yaw, m_Rotation.pitch, m_Rotation.roll );
	
	m_Transform.SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.yaw, m_Rotation.pitch, m_Rotation.roll );
	m_Camera->GetTransform().SetRotation( RotationQuat );
}

void HCameraBoomComponent::UpdateCameraYaw( float YawDelta )
{
	m_Rotation.yaw += -(YawDelta * m_CameraYawSpeed) * GEngine->GetDeltaTime();
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.yaw, m_Rotation.pitch, m_Rotation.roll );
	m_Transform.SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.yaw, m_Rotation.pitch, m_Rotation.roll );
	m_Camera->GetTransform().SetRotation( RotationQuat );
}

void HCameraBoomComponent::SetCamera( HCameraComponent* Camera )
{
	m_Camera = Camera;
	m_Camera->GetTransform().LinkTo( m_Transform );
	SetViewOffset( FVector3( 20.f, 10.f, -70.f ) );
	m_Camera->GetTransform().SetRotation( FAngles::Zero );
}

void HCameraBoomComponent::SetViewOffset( const FVector3& Offset )
{ 
	m_Camera->GetTransform().SetPosition( Offset );
	m_ViewOffset = Offset;
}
