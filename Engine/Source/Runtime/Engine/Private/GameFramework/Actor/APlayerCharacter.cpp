// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APlayerCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"


APlayerCharacter::APlayerCharacter( FActorInitArgs& InitArgs )
	: APawn( InitArgs )
	, m_CanRotateCamera( true )
{

	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "Player camera" ) );
	//m_pCameraComponent->AttachTo( m_pRootComponent );
}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	FVector3 PlayerStartPos( -4.8f, 10.f, -106.4f );
	GetRootComponent()->SetPosition( PlayerStartPos );
	//m_pCameraComponent->SetPosition( 0.f, 0.f, -100.f );
}

void APlayerCharacter::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );


}

void APlayerCharacter::LookUp( float Value )
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

void APlayerCharacter::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		m_Rotation.y += Value * m_CameraYawSpeedMultiplier * GetWorld()->GetDeltaTime();
		m_RotY = FQuat::CreateFromAxisAngle( FVector3::Up, m_Rotation.y );
		m_pCameraComponent->SetRotation( m_RotX * m_RotY );

		//m_pCameraComponent->LookAt( m_pRootComponent->GetPosition() );
	}
}

void APlayerCharacter::TogglePitchYawRotation()
{
	m_CanRotateCamera = !m_CanRotateCamera;
}

void APlayerCharacter::SetupController( HControllerComponent& Controller )
{
	Super::SetupController( Controller );

	// Setup event callbacks for camera movement.
	//
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &APlayerCharacter::ThirdPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &APlayerCharacter::ThirdPersonMoveRight );
	//Controller.BindAxis( "MoveUp", this, &APawn::MoveUp );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );
	//Controller.BindAxis( "MouseWheelUp", (APawn*)this, &APawn::MoveForward );

	// Camera
	Controller.BindAction( "CameraPitchYawLock", IE_Pressed, this, &APlayerCharacter::TogglePitchYawRotation );
	Controller.BindAction( "CameraPitchYawLock", IE_Released, this, &APlayerCharacter::TogglePitchYawRotation );
	Controller.BindAxis( "LookUp", this, &APlayerCharacter::LookUp );
	Controller.BindAxis( "LookRight", this, &APlayerCharacter::LookRight );
}

void APlayerCharacter::ThirdPersonMoveForward( float Delta )
{
	//SetRootComponent( m_pCameraComponent );
	Move( m_pCameraComponent->GetLocalForward(), Delta );
}

void APlayerCharacter::ThirdPersonMoveRight( float Delta )
{
	//SetRootComponent( m_pCameraComponent );
	Move( m_pCameraComponent->GetLocalRight(), Delta );
}
