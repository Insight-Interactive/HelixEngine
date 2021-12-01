#pragma once

#include "Engine/Engine.h"

#include "Panels/MenuBarPanel.h"
#include "Panels/ToolbarPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/ConsoleOutputPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/WorldOutlinePanel.h"
#include "Panels/DetailsPanel.h"
#include "Tools/AssetImporter.h"
#include "Developer/ConsoleWindow.h"
#include "Editor/PreferencesViewport.h"


class ADebugPawn;
class KeyPressedEvent;
class KeyReleasedEvent;
class MousePositionMovedEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseWheelScrolledEvent;
class WindowResizeEvent;
class WindowLostFocusEvent;
class WindowFocusEvent;
class WindowFileDropEvent;
class AppBeginPlayEvent;
class AppEndPlayEvent;
class ObjectSelectedEvent;


struct EditorPreferences
{
	bool EnableDarkMode;
	float VerticalScrollSpeed;
	float HorizontalScrollSpeed;
	FVector3 DebugCameraPosition;
	FVector3 DebugCameraRotation;
	float DebugCameraPitchSpeed;
	float DebugCameraYawSpeed;
};


class HEditorEngine : public HEngine
{
	HE_DECL_NON_COPYABLE( HEditorEngine );
	using Super = HEngine;
public:
	HEditorEngine( CommandLine& CmdLine );
	virtual ~HEditorEngine();

	virtual void PreStartup() override;
	virtual void Startup() override;
	virtual void Shutdown() override;

	void EnableDarkMode( bool Enabled );

protected:
	virtual void RenderClientViewport( float DeltaTime ) override;

	// Event Processing
	//
	virtual void OnEvent( Event& e ) override;
	bool OnKeyPressed( KeyPressedEvent& e );
	bool OnKeyReleased( KeyReleasedEvent& e );
	bool OnMousePositionMoved( MousePositionMovedEvent& e );
	bool OnMouseButtonPressed( MouseButtonPressedEvent& e );
	bool OnMouseButtonReleased( MouseButtonReleasedEvent& e );
	bool OnMouseWheelScrolled( MouseWheelScrolledEvent& e );
	bool OnWindowResized( WindowResizeEvent& e );
	bool OnWindowFocus( WindowFocusEvent& e );
	bool OnWindowLostFocus( WindowLostFocusEvent& e );
	bool OnClientWindowClosed( WindowClosedEvent& e );
	bool OnClientWindowDropFile( WindowFileDropEvent& e );
	bool OnAppBeginPlay( AppBeginPlayEvent& e );
	bool OnAppEndPlay( AppEndPlayEvent& e );
	bool OnObjectSelected( ObjectSelectedEvent& e );

	void OnExitMenuItem();
	void OnSaveMenuItem();
	void OnEditorPreferencesMenuItem();
	void OnEditorPreferencesViewportClosed();
	void OnReloadPipelineShaders();

protected:
	void LoadEditorPreferences();
	void SaveEditorPreferences();
	void SetupImGuiRenderBackend();
	void SetupEditorPanels();
	int32 TranslateMouseButton( DigitalInput MouseKeycode );
	void PackageGame();
	void RegisterEditorOnlyAssets();

protected:
	PreferencesViewport m_PreferencesViewport;
	EditorPreferences m_UserPreferences;
	std::vector<Panel*> m_EditorPanels;

	MenuBarPanel m_MenuBar;
	ToolbarPanel m_ToolbarPanel;
	ContentBrowserPanel m_ContentBrowserPanel;
	ConsoleOutputPanel m_ConsoleOutputPanel;
	SceneViewportPanel m_SceneViewport;
	WorldOutlinePanel m_WorldOutline;
	DetailsPanel m_DetailsPanel;

	AssetImporter m_AssetImporter;
	ConsoleWindow m_ConsoleWindow;

};
