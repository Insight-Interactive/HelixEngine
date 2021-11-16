// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonStructs.h"

#include "ModelManager.h"
#include "Engine/Window.h"

#include "Input/InputDispatcher.h"
#include "Renderer/SceneRenderer.h"

// TEMP
#include "Renderer/MaterialManager.h"


class HWorld;
class FDeferredShadingTech;;
class FPostProcessUber;
class FSkyboxPass;
class WindowFocusEvent;
class WindowLostFocusEvent;
class MouseRawPointerMovedEvent;
class MouseWheelScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class KeyPressedEvent;
class KeyReleasedEvent;


class FViewportContext
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class SceneViewportPanel;
public:
	FViewportContext();
	~FViewportContext();

	virtual void Initialize( const FWindow::Description& WindowDesc );
	virtual void Uninitialize();
	virtual void Update( float DeltaTime );
	virtual void Render();

	bool IsValid();

	FWindow& GetWindow();
	FColorBuffer* GetPreDisplayBuffer();
	FInputDispatcher* GetInputDispatcher();

	void SetWindowMode( const EWindowMode& Mode );
	FVector2 GetDimensions();
	void BringToFocus();
	bool HasFocus();
	void Show();
	void Hide();

	// Input
	//

	bool IsPressed( DigitalInput Key );
	bool IsFirstPressed( DigitalInput Key );
	bool IsReleased( DigitalInput Key );

	float GetMouseMoveDeltaX();
	float GetMouseMoveDeltaY();
	// Lock the mouse to the screen center and hide it.
	void LockMouseToScreenCenter();
	// Unlock the mouse from the screen center and show it.
	void UnlockMouseFromScreenCenter();
	// Show the mouse if it is hidden.
	void ShowMouse();
	// Hide the mouse if it is shown.
	void HideMouse();

	FViewPort& GetClientViewport();
	FRect& GetClientRect();
	HWorld& GetWorld();
	/*
		Set the world this viewport can render.
	*/
	void SetWorld( HWorld* pWorldToView );

protected:
	void PresentOneFrame();
	void InitializeRenderingResources();
	void ReloadRenderPipelines();

	void RenderWorld(FColorBuffer& RenderTarget );

	// Event Processing
	//
	void OnEvent( Event& e );
	bool OnWindowLostFocus( WindowLostFocusEvent& e );
	bool OnWindowFocus( WindowFocusEvent& e );
	bool OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e );
	bool OnMouseWheelScrolled( MouseWheelScrolledEvent& e );
	bool OnMouseButtonPressed( MouseButtonPressedEvent& e );
	bool OnMouseButtonReleased( MouseButtonReleasedEvent& e );
	bool OnKeyPressed( KeyPressedEvent& e );
	bool OnKeyReleased( KeyReleasedEvent& e );

protected:
	FWindow m_Window;
	FInputDispatcher m_InputDispatcher;
	HWorld* m_WorldInView;

	// Rendering Resources
	//
	FSceneRenderer m_SceneRenderer;
	// FWindow
	FViewPort	m_ViewPort;
	FRect		m_ScissorRect;

	FColorBuffer m_SceneRenderTarget;
	FDepthBuffer m_DepthBuffer;

};


//
// Inline function implementation
//

FORCEINLINE bool FViewportContext::IsValid()
{
	return GetWindow().IsValid();
}

FORCEINLINE HWorld& FViewportContext::GetWorld()
{
	return *m_WorldInView;
}

FORCEINLINE void FViewportContext::SetWorld( HWorld* pWorldToView )
{
	m_WorldInView = pWorldToView;
}

FORCEINLINE FInputDispatcher* FViewportContext::GetInputDispatcher()
{
	return &m_InputDispatcher;
}

FORCEINLINE void FViewportContext::SetWindowMode( const EWindowMode& Mode )
{
	m_Window.SetWindowMode( Mode );
}

FORCEINLINE void FViewportContext::ShowMouse()
{
	m_InputDispatcher.GetInputSureyor().ShowMouse();
}

FORCEINLINE void FViewportContext::HideMouse()
{
	m_InputDispatcher.GetInputSureyor().HideMouse();
}

FORCEINLINE void FViewportContext::LockMouseToScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().AcquireMouse();
}

FORCEINLINE void FViewportContext::UnlockMouseFromScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().UnacquireMouse();
}

FORCEINLINE void FViewportContext::PresentOneFrame()
{
	m_Window.PresentOneFrame();
}

FORCEINLINE FVector2 FViewportContext::GetDimensions()
{
	return GetWindow().GetDimensions();
}

FORCEINLINE void FViewportContext::BringToFocus()
{
	GetWindow().SetFocus();
}

FORCEINLINE bool FViewportContext::HasFocus()
{
	return m_Window.HasFocus();
}

FORCEINLINE void FViewportContext::Show()
{
	m_Window.Show();
}

FORCEINLINE void FViewportContext::Hide()
{
	m_Window.Hide();
}

FORCEINLINE FWindow& FViewportContext::GetWindow()
{
	return m_Window;
}

FORCEINLINE FColorBuffer* FViewportContext::GetPreDisplayBuffer()
{
	return &m_SceneRenderTarget;
}

FORCEINLINE FViewPort& FViewportContext::GetClientViewport()
{
	return m_ViewPort;
}

FORCEINLINE FRect& FViewportContext::GetClientRect()
{
	return m_ScissorRect;
}

FORCEINLINE bool FViewportContext::IsPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsPressed( Key );
}

FORCEINLINE bool FViewportContext::IsFirstPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsFirstPressed( Key );
}

FORCEINLINE bool FViewportContext::IsReleased( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsReleased( Key );
}

FORCEINLINE float FViewportContext::GetMouseMoveDeltaX()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaX();
}

FORCEINLINE float FViewportContext::GetMouseMoveDeltaY()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaY();
}
