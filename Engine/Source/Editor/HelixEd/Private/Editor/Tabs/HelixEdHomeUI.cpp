#include "HelixEdPCH.h"

#include "Editor/Tabs/HelixEdHomeUI.h"

#include "Editor/EditorEngine.h"
#include "Developer/ADebugActor.h"
#include "Engine/Event/EngineEvent.h"
#include "Editor/Event/EditorEvent.h"
#include "GameFramework/Components/HFirstPersonCameraComponent.h"


HelixEdHomeUI::HelixEdHomeUI( FViewportContext& Owner )
	: m_OwningViewport( Owner )
{
}

HelixEdHomeUI::~HelixEdHomeUI()
{

}

void HelixEdHomeUI::SetupPanels()
{
	m_MenuBar.Initialize( &m_OwningViewport );
	m_ToolbarPanel.Initialize( &m_OwningViewport );
	m_SceneViewport.Initialize( &m_OwningViewport );
	m_ConsoleOutputPanel.Initialize( &m_OwningViewport );
	m_ContentBrowserPanel.Initialize( &m_OwningViewport );
	m_WorldOutline.Initialize( &m_OwningViewport );
	m_DetailsPanel.Initialize( &m_OwningViewport );

	m_MenuBar.AddMenuItem( "File", "Exit", this, &HelixEdHomeUI::OnExitMenuItem );
	//m_MenuBar.AddMenuItem( "File", "Save", this, &HelixEdHomeUI::OnSaveMenuItem );
	m_MenuBar.AddMenuItem( "File", "Package Game", GEditorEngine, &HEditorEngine::PackageGame );
	m_MenuBar.AddMenuItem( "Edit", "Preferences", GEditorEngine, &HEditorEngine::OnEditorPreferencesMenuItem );
	//m_MenuBar.AddMenuItem( "Developer", "Reload Pipeline Shaders", this, &HelixEdHomeUI::OnReloadPipelineShaders );
	m_MenuBar.AddMenuItem( "Editor", "Launch Standalone Instance", this, &HelixEdHomeUI::OnLaunchStandalone );

	m_ToolbarPanel.AddListener( this, &HelixEdHomeUI::OnEvent );
	ADebugActor* pDebugPawn = m_SceneViewport.GetDebugPawn();
	HFirstPersonCameraComponent* pDebugCamera = pDebugPawn->GetCameraComponent();
	pDebugPawn->GetTransform().SetPosition( GEditorEngine->GetPreferences().DebugCameraPosition );
	pDebugCamera->SetCameraAngles( GEditorEngine->GetPreferences().DebugCameraRotation );

	m_WorldOutline.AddListener( this, &HelixEdHomeUI::OnEvent );
	m_WorldOutline.SetWorld( &GEditorEngine->GetGameWorld() );
	m_ContentBrowserPanel.AddListener( this, &HelixEdHomeUI::OnEvent );
}

void HelixEdHomeUI::Tick( float DeltaTime )
{
	m_MenuBar.Tick( DeltaTime );
	m_ToolbarPanel.Tick( DeltaTime );
	m_SceneViewport.Tick( DeltaTime );
	m_ConsoleOutputPanel.Tick( DeltaTime );
	m_ContentBrowserPanel.Tick( DeltaTime );
	m_WorldOutline.Tick( DeltaTime );
	m_DetailsPanel.Tick( DeltaTime );
}


void HelixEdHomeUI::Render( FCommandContext& CmdContext )
{
	m_MenuBar.Render( CmdContext );
	m_ToolbarPanel.Render( CmdContext );
	m_SceneViewport.Render( CmdContext );
	m_ConsoleOutputPanel.Render( CmdContext );
	m_ContentBrowserPanel.Render( CmdContext );
	m_WorldOutline.Render( CmdContext );
	m_DetailsPanel.Render( CmdContext );
}


// Event Handlers
//

void HelixEdHomeUI::OnEvent( Event& e )
{
	EventDispatcher Dispatcher( e );

	// Application/Engine
	Dispatcher.Dispatch<EngineBeginPlayEvent>( this, &HelixEdHomeUI::OnAppBeginPlay );
	Dispatcher.Dispatch<EngineEndPlayEvent>( this, &HelixEdHomeUI::OnAppEndPlay );
	Dispatcher.Dispatch<ObjectSelectedEvent>( this, &HelixEdHomeUI::OnObjectSelected );
	Dispatcher.Dispatch<ContentItemDoubleClicked>( GEditorEngine, &HEditorEngine::OnContentItemClicked );

	m_SceneViewport.OnEvent( e );
}

bool HelixEdHomeUI::OnAppBeginPlay( EngineBeginPlayEvent& e )
{
	OnAppBeginPlay();
	GEditorEngine->OnAppBeginPlay( e );
	m_DetailsPanel.SetSelectedObject( nullptr );

	return false;
}

bool HelixEdHomeUI::OnAppEndPlay( EngineEndPlayEvent& e )
{
	OnAppEndPlay();
	GEditorEngine->OnAppEndPlay( e );
	m_DetailsPanel.SetSelectedObject( nullptr );
	return false;
}

bool HelixEdHomeUI::OnObjectSelected( ObjectSelectedEvent& e )
{
	SetSelectedObject( e.GetSelectedObject() );
	GEditorEngine->OnObjectSelected( e );
	return false;
}

void HelixEdHomeUI::OnExitMenuItem()
{

}

void HelixEdHomeUI::OnSaveMenuItem()
{
	GEditorEngine->GetGameWorld().Save();
}

void HelixEdHomeUI::OnEditorPreferencesMenuItem()
{

}

void HelixEdHomeUI::OnEditorPreferencesViewportClosed()
{

}

void HelixEdHomeUI::OnReloadPipelineShaders()
{

}

void HelixEdHomeUI::OnLaunchStandalone()
{

}
