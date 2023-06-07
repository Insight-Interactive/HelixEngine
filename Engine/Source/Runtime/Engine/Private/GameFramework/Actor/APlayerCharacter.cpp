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
	m_pCameraComponent->AttachTo(GetRootComponent());

	m_pRootComponent->SetPosition( 0.f, 0.f, -28.f );
}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetRootComponent()->SetPosition( { -4.8f, 4.2f, -106.4f} );

}

void APlayerCharacter::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );
}

void APlayerCharacter::LookUp( float Value )
{
	if (m_CanRotateCamera)
	{
		const float AppliedRotation = Value * m_CameraPitchSpeedMultiplier* GetWorld()->GetDeltaTime();
		m_pCameraComponent->Rotate( AppliedRotation, 0.0f, 0.0f );
		m_pRootComponent->SetRotation( m_pCameraComponent->GetRotation() );

		// Rotation Clamp
		const float kXRotationClamp = 1.5f;
		FVector3 Rotation = m_pCameraComponent->GetEulerRotation();
		if (Rotation.x < -1.501f)
		{
			m_pCameraComponent->SetRotation( -kXRotationClamp, Rotation.y, Rotation.z );
		}
		else if (Rotation.x > 1.501f)
		{
			m_pCameraComponent->SetRotation( kXRotationClamp, Rotation.y, Rotation.z );
		}
	}
}

void APlayerCharacter::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		const float Rotation = Value * m_CameraYawSpeedMultiplier * GetWorld()->GetDeltaTime();
		m_pCameraComponent->Rotate( 0.0f, Rotation, 0.0f );
		m_pRootComponent->SetRotation( m_pCameraComponent->GetRotation() );
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
	Controller.BindAxis( "MoveForward", this, &APawn::MoveForward );
	Controller.BindAxis( "MoveRight", this, &APawn::MoveRight );
	Controller.BindAxis( "MoveUp", this, &APawn::MoveUp );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );
	Controller.BindAxis( "MouseWheelUp", (APawn*)this, &APawn::MoveForward );

	// Camera
	Controller.BindAction( "CameraPitchYawLock", IE_Pressed, this, &APlayerCharacter::TogglePitchYawRotation );
	Controller.BindAction( "CameraPitchYawLock", IE_Released, this, &APlayerCharacter::TogglePitchYawRotation );
	Controller.BindAxis( "LookUp", this, &APlayerCharacter::LookUp );
	Controller.BindAxis( "LookRight", this, &APlayerCharacter::LookRight );
}
