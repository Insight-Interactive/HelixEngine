#pragma once

#include "Engine/HEngine.h"

#include "Panels/SceneViewportPanel.h"


class KeyPressedEvent;
class KeyReleasedEvent;
class MousePositionMovedEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class WindowResizeEvent;
class WindowLostFocusEvent;
class WindowFocusEvent;

class HEditorEngine : public HEngine
{
	HE_DECL_NON_COPYABLE( HEditorEngine );
public:
	HEditorEngine();
	virtual ~HEditorEngine();

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
	bool OnWindowResized( WindowResizeEvent& e );
	bool OnWindowFocus( WindowFocusEvent& e );
	bool OnWindowLostFocus( WindowLostFocusEvent& e );
	bool OnClientWindowClosed( WindowClosedEvent& e );

protected:
	void SetupImGuiRenderBackend();
	int32 TranslateMouseButton( DigitalInput MouseKeycode );

protected:
	std::vector<Panel*> m_EditorPanels;

	SceneViewportPanel m_SceneViewport;
};
