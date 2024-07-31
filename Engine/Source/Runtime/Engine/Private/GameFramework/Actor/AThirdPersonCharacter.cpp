#include "EnginePCH.h"

#include "GameFramework/Actor/AThirdPersonCharacter.h"

#include "World/World.h"
#include "AssetRegistry/AssetDatabase.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HCapsuleColliderComponent.h"


AThirdPersonCharacter::AThirdPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
	, m_IsAiming( false )
	, m_ADSTimeSeconds( 0.1f )
	, m_ADSFOVDegrees( 50.f )
{
	m_CameraBoom = AddComponent<HCameraBoomComponent>( "CameraBoom" );
	m_CameraBoom->AttachTo( m_RootComponent );
	m_CameraBoom->UseCameraCollision( false );

	m_CameraComponent = AddComponent<HCameraComponent>( "FollowCamera" );
	m_CameraBoom->SetCamera( m_CameraComponent );
	m_CameraFOV = m_CameraComponent->GetFieldOfView();

	m_Body = AddComponent<HStaticMeshComponent>( "CharacterBody" );
	m_Body->AttachTo( m_RootComponent );
	m_Body->SetMesh( FAssetDatabase::GetStaticMesh( "Capsule01.fbx" ) );
	m_Body->SetMaterial( FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" ) );

	m_CharacterBounds->SetDrawEnabled( true );
}

AThirdPersonCharacter::~AThirdPersonCharacter()
{

}

bool Pressed = false;
FVector3 CameraPos;
FVector3 HitPos;
void AThirdPersonCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Pressed)
	{
		FDebugLineRenderInfo LineInfo;
		LineInfo.Start = CameraPos;
		LineInfo.End = HitPos;
		LineInfo.Color = FColor::GreenOpaque;
		LineInfo.IgnoreDepth = false;
		GetWorld()->DrawDebugLine( LineInfo );
	}
}

void AThirdPersonCharacter::SetupController( HControllerComponent& Controller )
{
	Super::SetupController( Controller );

	// Setup event callbacks for movement.
	
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &AThirdPersonCharacter::ThirdPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &AThirdPersonCharacter::ThirdPersonMoveRight );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );
	
	// Camera
	Controller.BindAxis( "LookUp", m_CameraBoom, &HCameraBoomComponent::UpdateCameraPitch );
	Controller.BindAxis( "LookRight", m_CameraBoom, &HCameraBoomComponent::UpdateCameraYaw );

	// Action
	Controller.BindAction( "AimDownSight", IE_Pressed, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "AimDownSight", IE_Released, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "FireWeapon", IE_Held, this, &AThirdPersonCharacter::FireWeapon );
}

void AThirdPersonCharacter::ThirdPersonMoveForward( float Delta )
{
	Move( m_CameraComponent->GetLocalForward(), Delta );
}

void AThirdPersonCharacter::ThirdPersonMoveRight( float Delta )
{
	Move( m_CameraComponent->GetLocalRight(), Delta );
}

void AThirdPersonCharacter::AimDownSight()
{
	m_IsAiming = !m_IsAiming;

	// TODO BUG(Garrett): Zooming out mid-zooom will restart lerp timer!
	if (m_IsAiming)
	{
		m_CameraComponent->LerpFieldOfView( m_ADSFOVDegrees, m_ADSTimeSeconds );
	}
	else
	{
		m_CameraComponent->LerpFieldOfView( m_CameraFOV, m_ADSTimeSeconds );
	}
}

void AThirdPersonCharacter::FireWeapon()
{
	Pressed = true;
	FVector2 WindowDims( GetWorld()->GetWindowWidth(), GetWorld()->GetWindowHeight() );
	FVector2 ScreenFirePos = WindowDims / 2;

	FVector3 Direction = Math::WorldDirectionFromScreenPos( ScreenFirePos, WindowDims, m_CameraComponent->GetViewMatrix(), m_CameraComponent->GetProjectionMatrix(), m_CameraComponent->GetNearZ(), m_CameraComponent->GetFarZ() );
	CameraPos = GetWorld()->GetCurrentSceneRenderCamera()->GetWorldPosition();
	
	FRaycastHitInfo HitInfo = {};
	std::vector<HColliderComponent*> IgnoreActors; IgnoreActors.push_back( (HColliderComponent*)m_CharacterBounds );
	GetWorld()->Raycast( CameraPos, Direction, 1000.f, &HitInfo, &IgnoreActors );

	if (HitInfo.AnyHit)
		HitPos = HitInfo.HitPos;
	else
		HitPos = CameraPos + Direction * 1000.f;
}
