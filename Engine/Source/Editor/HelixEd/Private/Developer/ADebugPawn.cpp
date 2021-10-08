#include "HelixEdPCH.h"

#include "Developer/ADebugPawn.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "World/World.h"


ADebugPawn::ADebugPawn( HWorld* pWorld, const HName& Name )
	: APawn( pWorld, Name )
{
	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "Player camera" ) );
	m_pCameraComponent->GetTransform().SetParent( &m_Transform );
	//m_Transform.SetPosition( 0.f, 0.f, -3.f );
	m_CanMove = true;
}

ADebugPawn::~ADebugPawn()
{

}

void ADebugPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ADebugPawn::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );

	UpdateMovement( DeltaMs );
}

void ADebugPawn::UpdateMovement( float DeltaTime )
{
	if (m_CanRotateCamera)
	{
		float X = GetWorld()->GetOwningViewport()->GetMouseMoveDeltaX() * DeltaTime;
		float Y = GetWorld()->GetOwningViewport()->GetMouseMoveDeltaY() * DeltaTime;
		if (Y != 0.f)
		{
			LookUp( Y );
		}
		if (X != 0.f)
		{
			LookRight( X );
		}
	}

	if (m_CanMove)
	{
		FViewportContext* pViewport = GetWorld()->GetOwningViewport();
		if (pViewport->IsPressed( Key_W ))
		{
			MoveForward( 1.f );
		}
		if (pViewport->IsPressed( Key_S ))
		{
			MoveForward( -1.f );
		}
		if (pViewport->IsPressed( Key_D ))
		{
			MoveRight( 1.f );
		}
		if (pViewport->IsPressed( Key_A ))
		{
			MoveRight( -1.f );
		}
		if (pViewport->IsPressed( Key_E ))
		{
			MoveUp( 1.f );
		}
		if (pViewport->IsPressed( Key_Q ))
		{
			MoveUp( -1.f );
		}
	}
}

void ADebugPawn::LookUp( float Value )
{
	if (m_CanRotateCamera)
	{
		m_pCameraComponent->GetTransform().Rotate( Value * m_CameraPitchSpeedMultiplier * GetWorld()->GetDeltaTime(), 0.0f, 0.0f );
		m_Transform.SetRotation( m_pCameraComponent->GetTransform().GetRotation() );
	}
}

void ADebugPawn::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		m_pCameraComponent->GetTransform().Rotate( 0.0f, Value * m_CameraYawSpeedMultiplier * GetWorld()->GetDeltaTime(), 0.0f );
		m_Transform.SetRotation( m_pCameraComponent->GetTransform().GetRotation() );
	}
}

void ADebugPawn::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}
