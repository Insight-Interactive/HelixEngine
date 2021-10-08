// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonStructs.h"

#include "ModelManager.h"
#include "Engine/Window.h"

#include "Input/InputDispatcher.h"


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
	void SetCommonRenderState( FCommandContext& CmdContext );
	void ReloadRenderPipelines();

	void RenderWorld( FCommandContext& CmdContext, FColorBuffer& RenderTarget );

	FConstantBuffer* GetSceneConstBufferForCurrentFrame();
	FConstantBuffer* GetLightConstBufferForCurrentFrame();

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
	HWorld* m_WorldInView;

	FInputDispatcher m_InputDispatcher;

	// Rendering Resources
	//
	FDeferredShadingTech* m_DeferredShader;
	FSkyboxPass* m_SkyPass;
	FPostProcessUber* m_PostProcessPass;
	StaticMeshGeometryRef m_pScreenQuadRef;
	// Constant buffers
	std::vector<FConstantBuffer*> m_pSceneConstantBuffers;
	std::vector<FConstantBuffer*> m_pLightConstantBuffers;
	// FWindow
	FViewPort	m_ViewPort;
	FRect		m_ScissorRect;

	FColorBuffer* m_pSceneRenderTarget;

};


//
// Inline function implementation
//

inline bool FViewportContext::IsValid()
{
	return GetWindow().IsValid();
}

inline HWorld& FViewportContext::GetWorld()
{
	return *m_WorldInView;
}

inline void FViewportContext::SetWorld( HWorld* pWorldToView )
{
	m_WorldInView = pWorldToView;
}

inline FInputDispatcher* FViewportContext::GetInputDispatcher()
{
	return &m_InputDispatcher;
}

inline void FViewportContext::ShowMouse()
{
	m_InputDispatcher.GetInputSureyor().ShowMouse();
}

inline void FViewportContext::HideMouse()
{
	m_InputDispatcher.GetInputSureyor().HideMouse();
}

inline void FViewportContext::LockMouseToScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().AcquireMouse();
}

inline void FViewportContext::UnlockMouseFromScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().UnacquireMouse();
}

inline void FViewportContext::PresentOneFrame()
{
	m_Window.PresentOneFrame();
}

inline FVector2 FViewportContext::GetDimensions()
{
	return GetWindow().GetDimensions();
}

inline void FViewportContext::BringToFocus()
{
	GetWindow().SetFocus();
}

inline bool FViewportContext::HasFocus()
{
	return m_Window.HasFocus();
}

inline void FViewportContext::Show()
{
	m_Window.Show();
}

inline void FViewportContext::Hide()
{
	m_Window.Hide();
}

inline FWindow& FViewportContext::GetWindow()
{
	return m_Window;
}

inline FColorBuffer* FViewportContext::GetPreDisplayBuffer()
{
	return m_pSceneRenderTarget;
}

inline FConstantBuffer* FViewportContext::GetSceneConstBufferForCurrentFrame()
{
	return m_pSceneConstantBuffers[m_Window.GetSwapChain()->GetCurrentFrameIndex()];
}

inline FConstantBuffer* FViewportContext::GetLightConstBufferForCurrentFrame()
{
	return m_pLightConstantBuffers[m_Window.GetSwapChain()->GetCurrentFrameIndex()];
}

inline FViewPort& FViewportContext::GetClientViewport()
{
	return m_ViewPort;
}

inline FRect& FViewportContext::GetClientRect()
{
	return m_ScissorRect;
}

inline bool FViewportContext::IsPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsPressed( Key );
}

inline bool FViewportContext::IsFirstPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsFirstPressed( Key );
}

inline bool FViewportContext::IsReleased( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsReleased( Key );
}

inline float FViewportContext::GetMouseMoveDeltaX()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaX();
}

inline float FViewportContext::GetMouseMoveDeltaY()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaY();
}
