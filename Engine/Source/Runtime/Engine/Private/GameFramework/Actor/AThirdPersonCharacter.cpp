#include "EnginePCH.h"

#include "GameFramework/Actor/AThirdPersonCharacter.h"

#include "World/World.h"
#include "AssetRegistry/AssetDatabase.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"
#include "GameFramework/Components/HControllerComponent.h"


AThirdPersonCharacter::AThirdPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
	, m_IsAiming( false )
	, m_ADSTimeSeconds( 0.1f )
	, m_ADSFOVDegrees( 50.f )
{

	m_CameraBoom = AddComponent<HCameraBoomComponent>( "CameraBoom" );
	m_CameraBoom->GetTransform().LinkTo( m_Transform, FVector3(0.f, GetPawnHeight() * 0.5f, 0.f) );
	m_CameraBoom->UseCameraCollision( true );

	m_CameraComponent = AddComponent<HCameraComponent>( "FollowCamera" );
	m_CameraBoom->SetCamera( m_CameraComponent );
	m_CameraComponent->SetFieldOfView( 80.f );
	m_CameraBoom->SetViewOffset( FVector3( 60.f, 70.f, -180.f ) );
	m_CameraFOV = m_CameraComponent->GetFieldOfView();

	m_Body->SetMesh( FAssetDatabase::GetSkeletalMesh( "SK_Bot.fbx" ) );
	m_Body->SetMaterial( FAssetDatabase::GetMaterial( "M_Animated.hmat" ) );
	m_Body->SetAnimation( FAssetDatabase::GetAnimation( "BipedalLocamotion/Walk_Relaxed.fbx" ) );
}

AThirdPersonCharacter::~AThirdPersonCharacter()
{

}

bool Pressed = false;
FRaycastHitInfo HitInfo = {};
FVector3 CameraPos;
FVector3 HitPos;
void AThirdPersonCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	//FVector3 Pos = m_Transform.GetPosition();
	//HE_LOG( Log, TEXT( "%f, %f, %f" ), Pos.x, Pos.y, Pos.z );

	if (Pressed)
	{
		FDebugLineRenderInfo LineInfo;
		LineInfo.Start = CameraPos;
		LineInfo.End = HitPos;
		LineInfo.Color = HitInfo.AnyHit ? FColor::RedOpaque : FColor::WhiteOpaque;
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
	Controller.BindAction( "Jump", IE_Pressed, this, &APawn::Jump );
	
	// Camera
	Controller.BindAxis( "LookUp", m_CameraBoom, &HCameraBoomComponent::UpdateCameraPitch );
	Controller.BindAxis( "LookRight", m_CameraBoom, &HCameraBoomComponent::UpdateCameraYaw );

	// Action
	Controller.BindAction( "AimDownSight", IE_Pressed, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "AimDownSight", IE_Released, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "FireWeapon", IE_Held, this, &AThirdPersonCharacter::FireWeapon );
	Controller.BindAction( "Melee", IE_Pressed, this, &AThirdPersonCharacter::DoMelee );
}

void AThirdPersonCharacter::ThirdPersonMoveForward( float Delta )
{
	Move( m_CameraComponent->GetTransform().GetLocalForward(), Delta );
}

void AThirdPersonCharacter::ThirdPersonMoveRight( float Delta )
{
	Move( m_CameraComponent->GetTransform().GetLocalRight(), Delta );
}

void AThirdPersonCharacter::AimDownSight()
{
	m_IsAiming = !m_IsAiming;

	if (m_IsAiming)
		m_CameraComponent->LerpFieldOfView( m_ADSFOVDegrees, m_ADSTimeSeconds );
	else
		m_CameraComponent->LerpFieldOfView( m_CameraFOV, m_ADSTimeSeconds );
}

void AThirdPersonCharacter::FireWeapon()
{
	if (Pressed)
		return;

	Pressed = true;
	FVector2 WindowDims( GetWorld()->GetWindowWidth(), GetWorld()->GetWindowHeight() );
	FVector2 ScreenFirePos = WindowDims / 2;

	FVector3 Direction = Math::WorldDirectionFromScreenPos( ScreenFirePos, WindowDims, m_CameraComponent->GetViewMatrix(), m_CameraComponent->GetProjectionMatrix() );
	CameraPos = m_CameraComponent->GetTransform().GetWorldPosition();
	
	float MaxTraceDistance = 10000.f;
	GetWorld()->Raycast( CameraPos, Direction, MaxTraceDistance, &HitInfo );

	if (HitInfo.AnyHit)
		HitPos = HitInfo.HitPos;
	else
		HitPos = CameraPos + Direction * MaxTraceDistance;
}

void AThirdPersonCharacter::DoMelee()
{
	FRaycastHitInfo MeleeHitInfo = {};
	float MeleeDistance = 36.f;

	FVector3 StartPos = GetTransform().GetWorldPosition() + FVector3( 0.f, GetPawnHeight(), 0.f );
	GetWorld()->Raycast( StartPos, GetTransform().GetLocalForward(), MeleeDistance, &MeleeHitInfo );

}
