#include "HelixEdPCH.h"

#include "Editor/Tabs/ActorEditorTab.h"

#include "Engine/GameProject.h"
#include "Developer/ADebugActor.h"
#include "Engine/Renderer/CommandContext.h"
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
	m_Transform.SetPosition( FVector3( -40.f, 0.f, 0.f ) );
	m_Transform.SetScale( FVector3( 10.f, 10.f, 10.f ) );

	Mesh = AddComponent<HStaticMeshComponent>( "CubeMesh" );
	Mesh->SetMesh( FAssetDatabase::GetStaticMesh( "Cube.fbx" ) );
	Mesh->SetMaterial( FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" ) );

	Light = AddComponent<HPointLightComponent>( "PointLight" );
	Light->SetBrightness( 800.f );
	Light->SetColor( FColor( 0.f, 255.f, 0.f ) );
	Light->SetPosition( 8.f, 12.f, 0.f );

	//Light->GetTransform().LinkTo( m_Transform );
	Mesh->GetTransform().LinkTo( m_Transform );
}
ARotatingActor::~ARotatingActor()
{
}
void ARotatingActor::BeginPlay()
{
}
void ARotatingActor::Tick( float DeltaTime )
{
	Mesh->GetTransform().Rotate( 0.f, 0.005f, 0.f );
}

void ActorEditorTab::OnActivated()
{
	Super::OnActivated();

	m_ActorWorld.Initialize();

	m_SceneViewport.GetDebugPawn()->GetTransform().SetPosition( 0.f, 0.f, 20.f );
	m_ActorWorld.CreateDynamicActorinstance<ARotatingActor>( "Actor Editor Pawn" );
}

void ActorEditorTab::OnDeactivated()
{
	Super::OnDeactivated();
}

void ActorEditorTab::SetupPanels()
{
	m_SceneViewport.Initialize( this );
	m_ActorWorld.SetCurrentSceneRenderCamera( (HCameraComponent*)m_SceneViewport.GetDebugPawn()->GetCameraComponent() );

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
