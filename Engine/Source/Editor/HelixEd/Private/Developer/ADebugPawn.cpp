#include "HelixEdPCH.h"

#include "Developer/ADebugPawn.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "World/World.h"


ADebugPawn::ADebugPawn( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
{
	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "Debug Camera" ) );
	m_pCameraComponent->AttachTo(GetRootComponent());
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
		float X = GetWorld()->GetOwningViewport()->GetMouseMoveDeltaX();
		float Y = GetWorld()->GetOwningViewport()->GetMouseMoveDeltaY();
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
		m_pCameraComponent->Rotate( Value * m_CameraPitchSpeedMultiplier * GetWorld()->GetDeltaTime(), 0.0f, 0.0f );
		m_pRootComponent->SetRotation( m_pCameraComponent->GetRotation() );
	}
}

void ADebugPawn::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		m_pCameraComponent->Rotate( 0.0f, Value * m_CameraYawSpeedMultiplier * GetWorld()->GetDeltaTime(), 0.0f );
		m_pRootComponent->SetRotation(m_pCameraComponent->GetRotation() );
	}
}

void ADebugPawn::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}
