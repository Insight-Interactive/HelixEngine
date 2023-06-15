#pragma once

#include "Editor/Tabs/ContentEditorInterface.h"

#include "Panels/MenuBarPanel.h"
#include "Panels/ToolbarPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/ConsoleOutputPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/WorldOutlinePanel.h"
#include "Panels/DetailsPanel.h"


class Event;
class EngineBeginPlayEvent;
class EngineEndPlayEvent;
class ObjectSelectedEvent;

class HelixEdHomeUI 
{
	using Super = ContentEditorInterface;
	friend class HEditorEngine;
public:
	HelixEdHomeUI( FViewportContext& Owner );
	virtual ~HelixEdHomeUI();

	void Tick( float DeltaTime );
	void Render( FCommandContext& CmdContext );
	void OnEvent( Event& e );

	void OnAppBeginPlay();
	void OnAppEndPlay();

	void SetSelectedObject( HObject* pSelectedObject );
	void DeactivateDebugCamera();
	void ActivateDebugCamera();
	ADebugPawn* GetDebugPawn();

private:
	bool OnAppBeginPlay( EngineBeginPlayEvent& e );
	bool OnAppEndPlay( EngineEndPlayEvent& e );
	bool OnObjectSelected( ObjectSelectedEvent& e );

	void OnExitMenuItem();
	void OnSaveMenuItem();
	void OnEditorPreferencesMenuItem();
	void OnEditorPreferencesViewportClosed();
	void OnReloadPipelineShaders();
	void OnLaunchStandalone();

	void SetupPanels();

private:
	FViewportContext& m_OwningViewport;

	MenuBarPanel m_MenuBar;
	ToolbarPanel m_ToolbarPanel;
	ContentBrowserPanel m_ContentBrowserPanel;
	ConsoleOutputPanel m_ConsoleOutputPanel;
	SceneViewportPanel m_SceneViewport;
	WorldOutlinePanel m_WorldOutline;
	DetailsPanel m_DetailsPanel;

};


//
// Inline function implementations
//

inline void HelixEdHomeUI::OnAppBeginPlay()
{
	SetSelectedObject( nullptr );
	DeactivateDebugCamera();
}

inline void HelixEdHomeUI::OnAppEndPlay()
{
	ActivateDebugCamera();
}

inline void HelixEdHomeUI::SetSelectedObject( HObject* pSelectedObject )
{
	m_DetailsPanel.SetSelectedObject( pSelectedObject );
}

inline void HelixEdHomeUI::DeactivateDebugCamera()
{
	m_SceneViewport.DeactivateDebugCamera();
}

inline void HelixEdHomeUI::ActivateDebugCamera()
{
	m_SceneViewport.ActivateDebugCamera();
}

inline ADebugPawn* HelixEdHomeUI::GetDebugPawn()
{
	return m_SceneViewport.GetDebugPawn();
}
