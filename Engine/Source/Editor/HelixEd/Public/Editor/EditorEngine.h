// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Engine.h"

#include "UI/EditorUI.h"
#include "Editor/Tabs/ActorEditorTab.h"
#include "Editor/Tabs/HelixEdHomeUI.h"
#include "Editor/Tabs/PreferencesTab.h"
#include "Tools/AssetImporter.h"


class ADebugActor;
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
class EngineBeginPlayEvent;
class EngineEndPlayEvent;
class ObjectSelectedEvent;
class ContentItemDoubleClicked;


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
	friend class HelixEdHomeUI;
public:
	HEditorEngine( FCommandLine& CmdLine );
	virtual ~HEditorEngine();

	virtual void PreStartup() override;
	virtual void PostStartup() override;
	virtual void Startup() override;
	virtual void Shutdown() override;

	void EnableDarkMode( bool Enabled );

	void PackageGame();
	EditorPreferences& GetPreferences();

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
	bool OnAppBeginPlay( EngineBeginPlayEvent& e );
	bool OnAppEndPlay( EngineEndPlayEvent& e );
	bool OnObjectSelected( ObjectSelectedEvent& e );
	bool OnContentItemClicked( ContentItemDoubleClicked& e );

	void OnExitMenuItem();
	void OnSaveMenuItem();
	void OnEditorPreferencesMenuItem();
	void OnEditorPreferencesViewportClosed();
	void OnReloadPipelineShaders();
	void OnLaunchStandalone();

protected:
	void LoadEditorPreferences();
	void SaveEditorPreferences();
	int32 TranslateMouseButton( DigitalInput MouseKeycode );
	void RegisterEditorOnlyAssets();

protected:
	std::vector<ActorEditorTab*> m_ActorEditors;
	EditorUIContext m_UIContext;

	HelixEdHomeUI m_HomeUI;
	//PreferencesTab m_PreferencesTab;

	EditorPreferences m_UserPreferences;
	std::vector<ContentEditorInterface*> m_EditorTabs;


	AssetImporter m_AssetImporter;
	ConsoleWindow m_ConsoleWindow;

};

// Global editor engine reference. Can only be accessed in editor build configurations.
extern HEditorEngine* GEditorEngine;

//
// Inline function implementations
//

FORCEINLINE EditorPreferences& HEditorEngine::GetPreferences() 
{
	return m_UserPreferences;
}
