#pragma once

#include "Engine/HEngine.h"

#include "Panels/MenuBarPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Tools/AssetImporter.h"
#include "Debug/ConsoleWindow.h"


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

struct EditorPreferences
{
	bool EnableDarkMode;
	float VerticalScrollSpeedMultiplier;
	float HorizontalScrollSpeedMultiplier;
};


class HEditorEngine : public HEngine
{
	HE_DECL_NON_COPYABLE( HEditorEngine );
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

	void OnExitMenuItem( );
	
protected:
	void SetupImGuiRenderBackend();
	void SetupEditorPanels();
	int32 TranslateMouseButton( DigitalInput MouseKeycode );

protected:
	EditorPreferences m_UserPreferences;
	std::vector<Panel*> m_EditorPanels;

	MenuBarPanel m_MenuBar;
	ContentBrowserPanel m_ContentBrowserPanel;
	SceneViewportPanel m_SceneViewport;

	AssetImporter m_AssetImporter;
	ConsoleWindow m_ConsoleWindow;

};
