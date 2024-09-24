#include "HelixEdPCH.h"

#include "Developer/ADebugActor.h"

#include "GameFramework/Components/HFirstPersonCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "World/World.h"


ADebugActor::ADebugActor( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
	, m_CanRotateCamera( false ) // So camera doesn't spawn without input being down
	, m_CanMove( true )
{
	m_CameraComponent = AddComponent<HFirstPersonCameraComponent>( "Debug Camera" );
	m_CameraComponent->GetTransform().LinkTo(m_Transform);
}

ADebugActor::~ADebugActor()
{

}

void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADebugActor::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );

	UpdateMovement( DeltaMs );
}

void ADebugActor::UpdateMovement( float DeltaTime )
{
	if (m_CanRotateCamera)
	{
		float X = Input::GetMouseMoveDeltaX();
		float Y = Input::GetMouseMoveDeltaY();
		if (Y != 0.f)
		{
			m_CameraComponent->LookUp( Y );
		}
		if (X != 0.f)
		{
			m_CameraComponent->LookRight( X );
		}
	}

	if (m_CanMove)
	{
		HWorld* pWorld = GetWorld();
		if (Input::IsPressed( Key_W ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalForward(), 1.f );
		}
		if (Input::IsPressed( Key_S ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalForward(), -1.f );
		}
		if (Input::IsPressed( Key_D ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalRight(), 1.f );
		}
		if (Input::IsPressed( Key_A ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalRight(), -1.f );
		}
		if (Input::IsPressed( Key_E ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalUp(), 1.f );
		}
		if (Input::IsPressed( Key_Q ))
		{
			MoveManual( m_CameraComponent->GetTransform().GetLocalUp(), -1.f );
		}
	}
}

void ADebugActor::MoveManual( FVector3& Direction, float Value )
{
	FVector3 Position = m_Transform.GetPosition();
	Position = Position + Direction * Value;
	m_Transform.SetPosition( Position );
}

void ADebugActor::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}
