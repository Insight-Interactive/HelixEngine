#include "EnginePCH.h"

#include "GameFramework/Actor/AThirdPersonCharacter.h"

#include "World/World.h"
#include "Engine/Engine.h"
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
	m_CurrentWeapon = &m_Rifle;
	m_CurrentWeapon->m_Transform.LinkTo( m_Transform, FVector3( 0.f, 100, 32.f ) );

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
	
	FVector3 BodyWorldPos = m_Body->GetTransform().GetWorldPosition();
	FDebugLineRenderInfo LineInfo;
	LineInfo.Start = BodyWorldPos;
	LineInfo.End = BodyWorldPos + m_Body->GetTransform().GetLocalForward() * 100;
	LineInfo.Color = FColor::WhiteOpaque;
	LineInfo.IgnoreDepth = true;
	GetWorld()->DrawDebugLine( LineInfo );


	FVector3 CameraWorldPos = m_CameraComponent->GetTransform().GetWorldPosition();
	FVector3 CameraForward = m_CameraComponent->GetTransform().GetLocalForward();
	CameraForward.y = 0.f;
	LineInfo.Start = BodyWorldPos;
	LineInfo.End = BodyWorldPos + CameraForward * 100;
	LineInfo.Color = FColor::RedOpaque;
	LineInfo.IgnoreDepth = true;
	GetWorld()->DrawDebugLine( LineInfo );

	/*if (Pressed)
	{
		FDebugLineRenderInfo LineInfo;
		LineInfo.Start = CameraPos;
		LineInfo.End = HitPos;
		LineInfo.Color = HitInfo.AnyHit ? FColor::RedOpaque : FColor::WhiteOpaque;
		LineInfo.IgnoreDepth = false;
		GetWorld()->DrawDebugLine( LineInfo );
	}*/
}

FVector3 AThirdPersonCharacter::GetShootDirection()
{
	FVector2 WindowDims = GEngine->GetClientViewport().GetDimensions();
	FVector2 ScreenFirePos = WindowDims * 0.5f;
	FVector3 ScreenWorldPos = Physics::ScreenToWorldPos( ScreenFirePos );
	FVector3 Direction = ScreenWorldPos - m_CurrentWeapon->m_Transform.GetWorldPosition();
	Direction.Normalize();
	return Direction;
}

void AThirdPersonCharacter::SetupController( HControllerComponent& Controller )
{
	Super::SetupController( Controller );

	// Setup event callbacks for movement and actions.
	
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &AThirdPersonCharacter::ThirdPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &AThirdPersonCharacter::ThirdPersonMoveRight );
	Controller.BindAction( "Sprint", IE_Pressed, this, &AThirdPersonCharacter::ThirdPersonSprint );
	Controller.BindAction( "Sprint", IE_Released, this, &AThirdPersonCharacter::ThirdPersonSprint );
	Controller.BindAction( "Jump", IE_Pressed, this, &APawn::Jump );
	
	// Camera
	Controller.BindAxis( "LookUp", m_CameraBoom, &HCameraBoomComponent::UpdateCameraPitch );
	Controller.BindAxis( "LookRight", m_CameraBoom, &HCameraBoomComponent::UpdateCameraYaw );

	// Action
	Controller.BindAction( "AimDownSight", IE_Pressed, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "AimDownSight", IE_Released, this, &AThirdPersonCharacter::AimDownSight );
	Controller.BindAction( "FireWeapon", IE_Pressed, this, &AThirdPersonCharacter::FireWeaponPressed );
	Controller.BindAction( "FireWeapon", IE_Held, this, &AThirdPersonCharacter::FireWeaponHeld );
	Controller.BindAction( "FireWeapon", IE_Released, this, &AThirdPersonCharacter::FireWeaponReleased );
	Controller.BindAction( "ReloadWeapon", IE_Pressed, this, &AThirdPersonCharacter::ReloadWeapon );
	Controller.BindAction( "Melee", IE_Pressed, this, &AThirdPersonCharacter::DoMelee );
}

void AThirdPersonCharacter::ThirdPersonMoveForward( float Delta )
{
	if (!m_bIsSprinting)
	{
		if (Delta > 0.f)
			m_Body->SetAnimation( m_WalkRelaxed );
		else
			m_Body->SetAnimation( m_IdleRelaxed );
	}

	//Move( m_Transform.GetLocalForward(), Delta );
	const FVector3& MoveDirection = m_CameraComponent->GetTransform().GetLocalForward();
	Move( MoveDirection, Delta );
	

	FQuat CameraRotation = m_CameraComponent->GetTransform().GetRotation();
	FQuat Lerped = FQuat::Slerp( CameraRotation, m_Body->GetTransform().GetRotation(), GEngine->GetDeltaTime() );
	Lerped.x = 0.f;
	Lerped.z = 0.f;
	m_Body->GetTransform().SetRotation( Lerped );
}

void AThirdPersonCharacter::ThirdPersonMoveRight( float Delta )
{
	Move( m_CameraComponent->GetTransform().GetLocalRight(), Delta );
}

void AThirdPersonCharacter::ThirdPersonSprint()
{
	APawn::Sprint();

	if (m_bIsSprinting)
	{
		m_Body->SetAnimation( m_RunRelaxed );
	}
}

void AThirdPersonCharacter::AimDownSight()
{
	m_IsAiming = !m_IsAiming;

	if (m_IsAiming)
		m_CameraComponent->LerpFieldOfView( m_ADSFOVDegrees, m_ADSTimeSeconds );
	else
		m_CameraComponent->LerpFieldOfView( m_CameraFOV, m_ADSTimeSeconds );
}

void AThirdPersonCharacter::FireWeaponPressed()
{
	if ( m_CurrentWeapon )
		m_CurrentWeapon->OnFireInputPressed( GetShootDirection() );

	//Pressed = true;
	//CameraPos = m_CameraComponent->GetTransform().GetWorldPosition();
	//
	//float MaxTraceDistance = 10000.f;
	//GetWorld()->Raycast( CameraPos, Direction, MaxTraceDistance, &HitInfo );

	//if (HitInfo.AnyHit)
	//	HitPos = HitInfo.HitPos;
	//else
	//	HitPos = CameraPos + Direction * MaxTraceDistance;
}

void AThirdPersonCharacter::FireWeaponHeld()
{
	if (m_CurrentWeapon)
		m_CurrentWeapon->OnFireInputHeld( GetShootDirection() );
}

void AThirdPersonCharacter::FireWeaponReleased()
{
	if (m_CurrentWeapon)
		m_CurrentWeapon->OnFireInputReleased( GetShootDirection() );
}

void AThirdPersonCharacter::ReloadWeapon()
{
	if (m_CurrentWeapon)
		m_CurrentWeapon->Reload();
}

void AThirdPersonCharacter::DoMelee()
{
	FRaycastHitInfo MeleeHitInfo = {};
	float MeleeDistance = 36.f;

	FVector3 StartPos = GetTransform().GetWorldPosition() + FVector3( 0.f, GetPawnHeight(), 0.f );
	GetWorld()->Raycast( StartPos, GetTransform().GetLocalForward(), MeleeDistance, &MeleeHitInfo );

}
