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
	m_pCameraComponent = AddComponent<HFirstPersonCameraComponent>( TEXT( "Debug Camera" ) );
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
	if (m_CanRotateCamera)
	{
		float X = GetWorld()->GetMouseMoveDeltaX();
		float Y = GetWorld()->GetMouseMoveDeltaY();
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
		HWorld* pWorld = GetWorld();
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
