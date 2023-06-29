#include "EnginePCH.h"

#include "GameFramework/Actor/AFirstPersonCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"


AFirstPersonCharacter::AFirstPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
	, m_CanRotateCamera( true )
{
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
		const float kXRotationClamp = Math::DegreesToRadians(88.f);

		m_Rotation.x += Value * m_CameraPitchSpeedMultiplier * GetWorld()->GetDeltaTime();
		m_Rotation.x = Math::Clamp( m_Rotation.x, -kXRotationClamp, kXRotationClamp );
		m_RotX = FQuat::CreateFromAxisAngle( FVector3::Right, m_Rotation.x );
		m_pCameraComponent->SetRotation( m_RotX * m_RotY );
	}
}

void AFirstPersonCharacter::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		m_Rotation.y += Value * m_CameraYawSpeedMultiplier * GetWorld()->GetDeltaTime();
		m_RotY = FQuat::CreateFromAxisAngle( FVector3::Up, m_Rotation.y );
		m_pCameraComponent->SetRotation( m_RotX * m_RotY );
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
