#include "EnginePCH.h"

#include "GameFramework/Components/HCameraBoomComponenet.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "World/World.h"
#include "Engine/ViewportContext.h"


HCameraBoomComponent::HCameraBoomComponent( FComponentInitArgs& InitArgs )
	: Super( InitArgs )
	, m_UseCameraCollision( true )
	, m_CameraCollisionTraceDistance( 2.f )
	, m_CameraDistance( -50.f )
	, m_ViewOffset( 0.f, 0.f, 0.f ) //20.f, 10.f, 0.f
{
	SetPosition( 0.f, 0.f, m_CameraDistance );
}

HCameraBoomComponent::~HCameraBoomComponent()
{

}

void HCameraBoomComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//UpdateCameraPitch();
	//UpdateCameraYaw();

	// Player-to-camera collision detection
	//
	//FVector3 TraceStart = m_pRootComponent->GetWorldPosition();
	//FVector3 TraceEnd = m_pCameraComponent->GetWorldPosition();
	//FVector3 TraceDir = TraceStart - TraceEnd;

	//FVector3 TraceDirNormal = FVector3::Normalize( TraceDir );

	// Front trace
	//{
	//	FRaycastHitInfo HitInfo = {};
	//	std::vector<HColliderComponent*> IgnoreColliders;
	//	IgnoreColliders.push_back( (HColliderComponent*)m_pRootComponent );
	//	bool ForwardHit = GetWorld()->Raycast( TraceStart, TraceDirNormal, TraceDir.Length(), &HitInfo, &IgnoreColliders, FG_WorldGeometry );
	//	//bool BackwardHit = GetWorld()->Raycast( m_pCameraComponent->GetWorldPosition(), m_pCameraComponent->GetLocalBackward(), TraceDistance );

	//	if ( ForwardHit )
	//	{
	//		m_pCameraComponent->SetPosition( HitInfo.HitPos );

	//		/*FDebugLineRenderInfo info = {};
	//		info.Start = TraceStart;
	//		info.End = HitInfo.HitPos;
	//		info.Color = FColor::GreenOpaque;
	//		info.Lifetime = 20.f;
	//		GetWorld()->DrawDebugLine( info );*/
	//	}
	//} 

	/*FVector3 DesiredCameraPos = m_pCameraComponent->GetWorldPosition() + (m_pCameraComponent->GetLocalBackward() * 50);
	{
		float TraceDistance = 2.f;

		FRaycastHitInfo HitInfo = {};
		std::vector<HColliderComponent*> IgnoreColliders;
		IgnoreColliders.push_back( (HColliderComponent*)m_pRootComponent );
		bool BackwardHit = GetWorld()->Raycast( m_pCameraComponent->GetWorldPosition(), m_pCameraComponent->GetLocalBackward(), TraceDistance, &HitInfo, &IgnoreColliders, FG_WorldGeometry);

		if (!BackwardHit)
		{
			float LerpWeight = 1.f;
			FVector3 LerpedPos = FVector3::Lerp( m_pCameraComponent->GetWorldPosition(), DesiredCameraPos, LerpWeight );
			m_pCameraComponent->SetPosition( LerpedPos );
		}
	}*/
}

void HCameraBoomComponent::UpdateCameraPitch( float PitchDelta )
{
	//float PitchDelta = GetWorld()->GetMouseMoveDeltaY();
	FVector3 CameraPosPreRotation = GetPosition();
	const FVector3& LookAtPos = FVector3::Zero;

	float Rotation = -(PitchDelta * 2048) * GetWorld()->GetDeltaTime();
	Rotation = Math::DegreesToRadians( Rotation );
	float RotatedY = cosf( Rotation ) * (CameraPosPreRotation.y - LookAtPos.y) - sinf( Rotation ) * (CameraPosPreRotation.z - LookAtPos.z) + LookAtPos.y;
	float RotatedZ = sinf( Rotation ) * (CameraPosPreRotation.y - LookAtPos.y) + cosf( Rotation ) * (CameraPosPreRotation.z - LookAtPos.z) + LookAtPos.z;

	LookAt( LookAtPos + m_ViewOffset );
	SetPosition( CameraPosPreRotation.x, RotatedY, RotatedZ );

	HCameraComponent* pCamera = GetOwner()->GetComponent<HCameraComponent>();
	//pCamera->LookAt( LookAtPos + m_ViewOffset );
	pCamera->SetRotation( GetRotation() );

	if( m_UseCameraCollision )
	{
		bool TopHit = GetWorld()->Raycast( GetWorldPosition(), GetLocalUp(), m_CameraCollisionTraceDistance );
		bool BottomHit = GetWorld()->Raycast( GetWorldPosition(), GetLocalDown(), m_CameraCollisionTraceDistance );

		if (TopHit || BottomHit)
			SetPosition( CameraPosPreRotation );
	}
}

void HCameraBoomComponent::UpdateCameraYaw( float YawDelta )
{
	//float YawDelta = GetWorld()->GetMouseMoveDeltaX();
	FVector3 CameraPosPreRotation = GetPosition();
	const FVector3& LookAtPos = FVector3::Zero;

	float Rotation = (YawDelta * 2048) * GetWorld()->GetDeltaTime();
	Rotation = Math::DegreesToRadians( Rotation );
	float RotatedX = cosf( Rotation ) * (CameraPosPreRotation.x - LookAtPos.x) - sinf( Rotation ) * (CameraPosPreRotation.z - LookAtPos.z) + LookAtPos.x;
	float RotatedZ = sinf( Rotation ) * (CameraPosPreRotation.x - LookAtPos.x) + cosf( Rotation ) * (CameraPosPreRotation.z - LookAtPos.z) + LookAtPos.z;

	LookAt( LookAtPos + m_ViewOffset );
	SetPosition( RotatedX, CameraPosPreRotation.y, RotatedZ );

	HCameraComponent* pCamera = GetOwner()->GetComponent<HCameraComponent>();
	//pCamera->LookAt( LookAtPos + m_ViewOffset );
	pCamera->SetRotation( GetRotation() );

	if( m_UseCameraCollision )
	{
		bool LeftHit = GetWorld()->Raycast( GetWorldPosition(), GetLocalLeft(), m_CameraCollisionTraceDistance );
		bool RightHit = GetWorld()->Raycast( GetWorldPosition(), GetLocalRight(), m_CameraCollisionTraceDistance );

		if (LeftHit || RightHit)
			SetPosition( CameraPosPreRotation );
	}
}
