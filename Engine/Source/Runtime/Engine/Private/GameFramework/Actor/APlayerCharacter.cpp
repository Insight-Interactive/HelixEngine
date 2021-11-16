// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/APlayerCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HControllerComponent.h"


APlayerCharacter::APlayerCharacter( HWorld* pWorld, const HName& Name )
	: APawn( pWorld, Name )
	, m_CanRotateCamera( true )
{
	m_pCameraComponent = AddComponent<HCameraComponent>( TEXT( "Player camera" ) );

	m_pCameraComponent->GetTransform().SetParent( &m_Transform );
	m_Transform.SetPosition( 0.f, 0.f, -28.f );

}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick( float DeltaMs )
{
	Super::Tick( DeltaMs );
}

void APlayerCharacter::LookUp( float Value )
{
	if (m_CanRotateCamera)
	{
		FTransform& CameraTransform = m_pCameraComponent->GetTransform();
		CameraTransform.Rotate( Value * m_CameraPitchSpeedMultiplier * GetWorld()->GetDeltaTime(), 0.0f, 0.0f );
		m_Transform.SetRotation( CameraTransform.GetRotation() );
	}
}

void APlayerCharacter::LookRight( float Value )
{
	if (m_CanRotateCamera)
	{
		float DeltaTime = GetWorld()->GetDeltaTime();
		m_pCameraComponent->GetTransform().Rotate( 0.0f, Value * m_CameraYawSpeedMultiplier * DeltaTime, 0.0f );
		m_Transform.SetRotation( m_pCameraComponent->GetTransform().GetRotation() );
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
