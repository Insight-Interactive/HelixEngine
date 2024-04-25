#include "EnginePCH.h"

#include "GameFramework/Actor/AFirstPersonCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HFirstPersonCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"


AFirstPersonCharacter::AFirstPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
	, m_CanRotateCamera( true )
{
	m_pCameraComponent = AddComponent<HFirstPersonCameraComponent>( TEXT( "FirstPersonCameraComponent" ) );
	m_pCameraComponent->AttachTo( m_pRootComponent );
	m_pCameraComponent->SetPosition( FVector3::Zero );
}

AFirstPersonCharacter::~AFirstPersonCharacter()
{

}

void AFirstPersonCharacter::SetupController( HControllerComponent& Controller )
{
	Super::SetupController( Controller );

	// Setup event callbacks for camera movement.
	//
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &AFirstPersonCharacter::FirstPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &AFirstPersonCharacter::FirstPersonMoveRight );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );

	// Camera
	Controller.BindAxis( "LookUp", this, &AFirstPersonCharacter::LookUp );
	Controller.BindAxis( "LookRight", this, &AFirstPersonCharacter::LookRight );
}

void AFirstPersonCharacter::LookUp( float Value )
{
	if (m_CanRotateCamera)
	{
		SCast<HFirstPersonCameraComponent*>( m_pCameraComponent )->LookUp( Value );
	}
}

void AFirstPersonCharacter::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		SCast<HFirstPersonCameraComponent*>( m_pCameraComponent )->LookRight( Value );
	}
}

void AFirstPersonCharacter::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}

void AFirstPersonCharacter::FirstPersonMoveForward( float Delta )
{
	Move( m_pCameraComponent->GetLocalForward(), Delta );
}

void AFirstPersonCharacter::FirstPersonMoveRight( float Delta )
{
	Move( m_pCameraComponent->GetLocalRight(), Delta );
}
