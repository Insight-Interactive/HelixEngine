#include "EnginePCH.h"

#include "GameFramework/Actor/AThirdPersonCharacter.h"

#include "World/World.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "GameFramework/Components/HCameraBoomComponenet.h"
#include "GameFramework/Components/HControllerComponent.h"
#include "GameFramework/Components/HSkeletalMeshComponenet.h"
#include "AssetRegistry/AssetDatabase.h"


AThirdPersonCharacter::AThirdPersonCharacter( FActorInitArgs& InitArgs )
	: ACharacter( InitArgs )
{
	m_pCameraBoom = AddComponent<HCameraBoomComponent>( TEXT( "CameraBoom" ) );
	m_pCameraBoom->AttachTo( m_pRootComponent );
	
	m_pCameraComponent->AttachTo( m_pCameraBoom );
	m_pCameraComponent->SetObjectName( TEXT( "FollowCamera" ) );

	//m_pCharacterMesh = AddComponent<HSkeletalMeshComponent>( TEXT( "CharacterMesh" ) );
	//FGUID guid = FGUID::CreateFromString( "e1e1007c-f54e-4e84-9051-e6b7d0f02ff7" );
	//m_pCharacterMesh->SetSkeletalMesh( FAssetDatabase::GetSkeletalMesh( guid ) );
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

	// Setup event callbacks for camera movement.
	//
	// Locamotion
	Controller.BindAxis( "MoveForward", this, &AThirdPersonCharacter::ThirdPersonMoveForward );
	Controller.BindAxis( "MoveRight", this, &AThirdPersonCharacter::ThirdPersonMoveRight );
	Controller.BindAction( "Sprint", IE_Pressed, this, &APawn::Sprint );
	Controller.BindAction( "Sprint", IE_Released, this, &APawn::Sprint );
}


void AThirdPersonCharacter::ThirdPersonMoveForward( float Delta )
{
	Move( m_pCameraComponent->GetLocalForward(), Delta );
}

void AThirdPersonCharacter::ThirdPersonMoveRight( float Delta )
{
	Move( m_pCameraComponent->GetLocalRight(), Delta );
}
