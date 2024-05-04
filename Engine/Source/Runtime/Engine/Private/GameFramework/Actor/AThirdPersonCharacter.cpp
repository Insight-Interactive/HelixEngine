#include "EnginePCH.h"

#include "GameFramework/Actor/AThirdPersonCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"
#include "GameFramework/Components/HControllerComponent.h"


AThirdPersonCharacter::AThirdPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
	, m_IsAiming( false )
	, m_ADSTimeSeconds( 0.1f )
	, m_ADSFOVDegrees( 50.f )
	, m_PreADSFOV( 1.f )
{
	m_pCameraBoom = AddComponent<HCameraBoomComponent>( TEXT( "CameraBoom" ) );
	m_pCameraBoom->AttachTo( m_pRootComponent );
	
	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "ThirdPersonCamera" ) );
	m_pCameraComponent->AttachTo( m_pCameraBoom );
	m_pCameraComponent->SetObjectName( TEXT( "FollowCamera" ) );

}

AThirdPersonCharacter::~AThirdPersonCharacter()
{

}

void AThirdPersonCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AThirdPersonCharacter::SetupController( HControllerComponent& Controller )
{
	Super::SetupController( Controller );

	// Setup event callbacks for movement.
	//
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &AThirdPersonCharacter::ThirdPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &AThirdPersonCharacter::ThirdPersonMoveRight );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );
	
	// Camera
	Controller.BindAxis( "LookUp", m_pCameraBoom, &HCameraBoomComponent::UpdateCameraPitch );
	Controller.BindAxis( "LookRight", m_pCameraBoom, &HCameraBoomComponent::UpdateCameraYaw );
	Controller.BindAction( "AimDownSight", IE_Pressed, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "AimDownSight", IE_Released, this, &AThirdPersonCharacter::AimDownSight );
}


void AThirdPersonCharacter::ThirdPersonMoveForward( float Delta )
{
	Move( m_pCameraComponent->GetLocalForward(), Delta );
}

void AThirdPersonCharacter::ThirdPersonMoveRight( float Delta )
{
	Move( m_pCameraComponent->GetLocalRight(), Delta );
}

void AThirdPersonCharacter::AimDownSight()
{
	m_IsAiming = !m_IsAiming;

	// TODO: Spamming ads will break fov
	if (m_IsAiming)
	{
		m_PreADSFOV = m_pCameraComponent->GetFieldOfView();
		m_pCameraComponent->LerpFieldOfView( m_ADSFOVDegrees, m_ADSTimeSeconds );
	}
	else
	{
		m_pCameraComponent->LerpFieldOfView( m_PreADSFOV, m_ADSTimeSeconds );
	}
}
