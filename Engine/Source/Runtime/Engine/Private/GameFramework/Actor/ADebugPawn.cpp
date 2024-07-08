#include "EnginePCH.h"

#include "GameFramework/Actor/ADebugPawn.h"

#include "GameFramework/Components/HFirstPersonCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "World/World.h"


ADebugPawn::ADebugPawn( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
	, m_CanRotateCamera( false ) // So camera doesn't spawn without input being down
	, m_CanMove( true )
{
	m_pCameraComponent = AddComponent<HFirstPersonCameraComponent>( "Debug Camera" );
	m_pCameraComponent->AttachTo(GetRootComponent());
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
	HWorld* pWorld = GetWorld();

	if (m_CanRotateCamera)
	{
		// Handle camera rotation
		float X = pWorld->GetMouseMoveDeltaX();
		float Y = pWorld->GetMouseMoveDeltaY();
		if (Y != 0.f)
		{
			m_pCameraComponent->LookUp( Y );
		}
		if (X != 0.f)
		{
			m_pCameraComponent->LookRight( X );
		}
	}

	if (m_CanMove)
	{
		// Handle sprint
		if (pWorld->IsFirstPressed( Key_LShift ))
		{
			Sprint();
		}
		if (pWorld->IsReleased( Key_LShift ))
		{
			if (m_bIsSprinting)
				Sprint();
		}

		// Handle movement
		if (pWorld->IsPressed( Key_W ))
		{
			MoveForward( 1.f );
		}
		if (pWorld->IsPressed( Key_S ))
		{
			MoveForward( -1.f );
		}
		if (pWorld->IsPressed( Key_D ))
		{
			MoveRight( 1.f );
		}
		if (pWorld->IsPressed( Key_A ))
		{
			MoveRight( -1.f );
		}
		if (pWorld->IsPressed( Key_E ))
		{
			MoveUp( 1.f );
		}
		if (pWorld->IsPressed( Key_Q ))
		{
			MoveUp( -1.f );
		}
	}
}

void ADebugPawn::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}
