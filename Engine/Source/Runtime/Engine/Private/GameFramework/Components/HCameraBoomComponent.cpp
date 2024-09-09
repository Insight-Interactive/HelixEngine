#include "EnginePCH.h"

#include "GameFramework/Components/HCameraBoomComponenet.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/ViewportContext.h"


HCameraBoomComponent::HCameraBoomComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
	, m_UseCameraCollision( true )
	, m_CameraCollisionTraceDistance( 5.f )
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
	m_Rotation.x += -(PitchDelta * 25.f) * GEngine->GetDeltaTime();
	m_Rotation.x = Math::Clamp( m_Rotation.x, -kPitchRotationClamp, kPitchRotationClamp );
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	m_Camera->SetRotation( RotationQuat );
}

void HCameraBoomComponent::UpdateCameraYaw( float YawDelta )
{
	m_Rotation.y += -(YawDelta * 25.f) * GEngine->GetDeltaTime();
	FQuat RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	SetRotation( RotationQuat );
	
	RotationQuat = FQuat::CreateFromYawPitchRoll( m_Rotation.y, m_Rotation.x, m_Rotation.z );
	m_Camera->SetRotation( RotationQuat );
}

void HCameraBoomComponent::SetCamera( HCameraComponent* Camera )
{
	m_Camera = Camera;
	m_Camera->AttachTo( this );
	SetViewOffset( FVector3( 20.f, 10.f, -70.f ) );
	m_Camera->SetRotation( FVector3::Zero );
}

void HCameraBoomComponent::SetViewOffset( const FVector3& Offset )
{ 
	m_Camera->SetPosition( Offset );
	m_ViewOffset = Offset;
}
