#include "EnginePCH.h"

#include "GameFramework/Components/HCameraBoomComponenet.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"


HCameraBoomComponent::HCameraBoomComponent( FComponentInitArgs& InitArgs )
	: Super( InitArgs )
	, m_UseCameraCollision( true )
	, m_CameraCollisionTraceDistance( 5.f )
{
}

HCameraBoomComponent::~HCameraBoomComponent()
{

}

void HCameraBoomComponent::Update( float DeltaTime )
{

	/*if( m_UseCameraCollision )
	{
		FVector3 WorldPos = GetWorldPosition();
		FVector3 WorldPos = m_Camera->GetWorldPosition();
		bool TopHit = GetWorld()->Raycast( WorldPos, m_Camera->GetLocalUp(), m_CameraCollisionTraceDistance );
		bool BottomHit = GetWorld()->Raycast( WorldPos, m_Camera->GetLocalDown(), m_CameraCollisionTraceDistance );

		bool LeftHit = GetWorld()->Raycast( WorldPos, GetLocalLeft(), m_CameraCollisionTraceDistance );
		bool RightHit = GetWorld()->Raycast( WorldPos, GetLocalRight(), m_CameraCollisionTraceDistance );

		if (LeftHit || RightHit)
			SetPosition( PosPreRotation );
	}*/
}

void HCameraBoomComponent::UpdateCameraPitch( float PitchDelta )
{
	const float kPitchRotationClamp = Math::DegreesToRadians( 50.f );
	m_Rotation.x += -(PitchDelta * 50.f) * GEngine->GetDeltaTime();
	m_Rotation.x = Math::Clamp( m_Rotation.x, -kPitchRotationClamp, kPitchRotationClamp );
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	m_Camera->SetRotation( RotationQuat );
}

void HCameraBoomComponent::UpdateCameraYaw( float YawDelta )
{
	m_Rotation.y += -(YawDelta * 50.f) * GEngine->GetDeltaTime();
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	m_Camera->SetRotation( RotationQuat );
}

void HCameraBoomComponent::SetCamera( HCameraComponent* Camera )
{
	m_Camera = Camera;
	m_Camera->AttachTo( this );
	m_Camera->SetPosition( 20.f, 10.f, -70.f );
	m_Camera->SetRotation( FVector3::Zero );
}

void HCameraBoomComponent::SetViewOffset( const FVector3& Offset )
{ 
	m_Camera->SetPosition( Offset );
}
