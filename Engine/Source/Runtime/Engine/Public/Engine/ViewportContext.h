// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonStructs.h"

#include "ModelManager.h"
#include "Engine/Window.h"

#include "Input/InputDispatcher.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/UIRenderer.h"


class HWorld;
class FSkyboxPass;
class FPostProcessUber;
class FDeferredShadingTech;;
class WindowFocusEvent;
class WindowLostFocusEvent;
class WindowResizeEvent;
class MouseWheelScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseRawPointerMovedEvent;
class KeyPressedEvent;
class KeyReleasedEvent;


class FViewportContext
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class SceneViewportPanel;
	friend class FRenderingSubsystem;
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
	FColorBuffer& GetMainSceneRenderTarget();
	FDepthBuffer& GetSceneDepthBuffer();
	FInputDispatcher* GetInputDispatcher();

	void SetWindowMode( const EWindowMode& Mode );
	FVector2 GetDimensions();
	void BringToFocus();
	bool HasFocus();
	void Show();
	void Hide();
	uint32 GetFrameIndex();

	FSceneRenderer& GetSceneRenderer();
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

	// Rendering
	//
	void RenderWorld(FColorBuffer& RenderTarget );
	void RenderUI( FColorBuffer& RenderTarget );

	// Event Processing
	//
	void OnEvent( Event& e );
	bool OnWindowLostFocus( WindowLostFocusEvent& e );
	bool OnWindowFocus( WindowFocusEvent& e );
	bool OnWindowResize( WindowResizeEvent& e );
	bool OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e );
	bool OnMouseWheelScrolled( MouseWheelScrolledEvent& e );
	bool OnMouseButtonPressed( MouseButtonPressedEvent& e );
	bool OnMouseButtonReleased( MouseButtonReleasedEvent& e );
	bool OnKeyPressed( KeyPressedEvent& e );
	bool OnKeyReleased( KeyReleasedEvent& e );

protected:
	FWindow m_Window;
	FInputDispatcher m_InputDispatcher;
	HWorld* m_pWorldInView;

	// Rendering Resources
	//
	FSceneRenderer m_SceneRenderer;
	FUIRenderer m_UIRenderer;
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
	return *m_pWorldInView;
}

FORCEINLINE FSceneRenderer& FViewportContext::GetSceneRenderer()
{
	return m_SceneRenderer;
}

FORCEINLINE FInputDispatcher* FViewportContext::GetInputDispatcher()
{
	return &m_InputDispatcher;
}

FORCEINLINE void FViewportContext::SetWindowMode( const EWindowMode& Mode )
{
	m_Window.SetWindowMode( Mode );
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

FORCEINLINE uint32 FViewportContext::GetFrameIndex()
{
	return GetWindow().GetSwapChain()->GetCurrentFrameIndex();
}

FORCEINLINE FWindow& FViewportContext::GetWindow()
{
	return m_Window;
}

FORCEINLINE FColorBuffer* FViewportContext::GetPreDisplayBuffer()
{
	return &m_SceneRenderTarget;
}

FORCEINLINE FDepthBuffer& FViewportContext::GetSceneDepthBuffer()
{
	return m_DepthBuffer;
}

FORCEINLINE FViewPort& FViewportContext::GetClientViewport()
{
	return m_ViewPort;
}

FORCEINLINE FRect& FViewportContext::GetClientRect()
{
	return m_ScissorRect;
}
