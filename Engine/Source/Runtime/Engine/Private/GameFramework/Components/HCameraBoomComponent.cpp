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
		

		/*FVector3 WorldPos = GetWorldPosition();
		FVector3 CameraPos = m_Camera->GetWorldPosition();
		FVector3 Direction = WorldPos - CameraPos;
		Direction.Normalize();
		FRaycastHitInfo HitInfo = {};
		GetWorld()->Raycast( WorldPos, Direction, Direction.Length(), &HitInfo );

		FDebugLineRenderInfo LineInfo;
		LineInfo.Start = GetWorldPosition();
		LineInfo.End = HitInfo.HitPos;
		LineInfo.Color = FColor::BlueOpaque;
		LineInfo.IgnoreDepth = false;
		GetWorld()->DrawDebugLine( LineInfo );*/


		/*bool TopHit = GetWorld()->Raycast( WorldPos, m_Camera->GetLocalUp(), m_CameraCollisionTraceDistance );
		bool BottomHit = GetWorld()->Raycast( WorldPos, m_Camera->GetLocalDown(), m_CameraCollisionTraceDistance );

		bool LeftHit = GetWorld()->Raycast( WorldPos, GetLocalLeft(), m_CameraCollisionTraceDistance );
		bool RightHit = GetWorld()->Raycast( WorldPos, GetLocalRight(), m_CameraCollisionTraceDistance );

		if (LeftHit || RightHit)
			SetPosition( PosPreRotation );*/
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
