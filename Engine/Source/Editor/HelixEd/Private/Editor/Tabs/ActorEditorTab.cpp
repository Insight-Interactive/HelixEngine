#include "HelixEdPCH.h"

#include "Editor/Tabs/ActorEditorTab.h"

#include "Engine/GameProject.h"
#include "Developer/ADebugPawn.h"
#include "CommandContext.h"
#include "Engine/ViewportContext.h"
#include "AssetRegistry/AssetDatabase.h"


ActorEditorTab::ActorEditorTab( const TChar* ActorName, HEditorEngine& EditorEngine )
	: ContentEditorInterface( ActorName )
{
	m_ActorWorld.SetViewport( this );
}

ActorEditorTab::~ActorEditorTab()
{
}


class ARotatingActor : public AActor
{
public:
	HE_GENERATED_BODY( ARotatingActor )

		virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

private:
	HStaticMeshComponent* Mesh;
	HPointLightComponent* Light;

};
ARotatingActor::ARotatingActor( FActorInitArgs& InitArgs )
	: AActor( InitArgs )
{
	m_pRootComponent = AddComponent<HSceneComponent>( TEXT( "Root" ) );

	m_pRootComponent->SetPosition( FVector3( -40.f, 0.f, 0.f ) );
	m_pRootComponent->SetScale( FVector3( 10.f, 10.f, 10.f ) );

	Mesh = AddComponent<HStaticMeshComponent>( TEXT( "CubeMesh" ) );
	Mesh->SetMesh( FAssetDatabase::GetStaticMesh( FGUID::CreateFromString( "4539421c-d8b4-4936-bb0c-8dde1e24f9b9" ) ) );
	Mesh->SetMaterial( FAssetDatabase::GetMaterial( FGUID::CreateFromString( "0d68e992-aa25-4aa4-9f81-0eb775320c1e" ) ) );

	Light = AddComponent<HPointLightComponent>( TEXT( "PointLight" ) );
	Light->SetBrightness( 800.f );
	Light->SetColor( FColor( 0.f, 255.f, 0.f ) );
	Light->SetPosition( 8.f, 12.f, 0.f );

	Light->AttachTo( m_pRootComponent );
	Mesh->AttachTo( m_pRootComponent );
}
ARotatingActor::~ARotatingActor()
{
}
void ARotatingActor::BeginPlay()
{
}
void ARotatingActor::Tick( float DeltaTime )
{
	Mesh->Rotate( 0.f, 0.005f, 0.f );
}

void ActorEditorTab::OnActivated()
{
	Super::OnActivated();

	m_ActorWorld.Initialize();

	m_SceneViewport.GetDebugPawn()->GetRootComponent()->SetPosition( 0.f, 0.f, 20.f );
	m_ActorWorld.CreateDynamicActorinstance<ARotatingActor>( TEXT( "Actor Editor Pawn" ) );
}

void ActorEditorTab::OnDeactivated()
{
	Super::OnDeactivated();
}

void ActorEditorTab::SetupPanels()
{
	m_SceneViewport.Initialize( this );
	m_ActorWorld.SetCurrentSceneRenderCamera( m_SceneViewport.GetDebugPawn()->GetCameraComponent() );

}

void ActorEditorTab::Tick( float DeltaTime )
{
	m_SceneViewport.Tick( DeltaTime );
}

void ActorEditorTab::Render( FCommandContext& CmdContext )
{
	Super::BeginNewFrame();

	ImGui::Begin( "TEST Window" );
	ImGui::Text( "Hello There!" );
	ImGui::End();

	m_SceneViewport.Render( CmdContext );

	Super::EndFrame( CmdContext );
}
