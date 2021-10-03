// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CommonStructs.h"

#include "ModelManager.h"
#include "Engine/Window.h"

#include "Input/InputDispatcher.h"


class HWorld;
class DeferredShadingTech;;
class PostProcesssUber;
class SkyboxPass;
class MouseRawPointerMovedEvent;


class ViewportContext
{
	friend class HEngine;
	friend class HEditorEngine;
public:
	ViewportContext();
	~ViewportContext();

	virtual void Initialize( const Window::Description& WindowDesc );
	virtual void Uninitialize();
	virtual void Update( float DeltaTime );
	virtual void Render();

	bool IsValid();

	Window& GetWindow();
	IColorBuffer* GetPreDisplayBuffer();
	InputDispatcher* GetInputDispatcher();
	
	void LockMouseToScreenCenter();
	void UnlockMouseFromScreenCenter();

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

	ViewPort& GetClientViewport();
	Rect& GetClientRect();
	HWorld& GetWorld();
	/*
		Set the world this viewport can render.
	*/
	void SetWorld( HWorld* pWorldToView );

protected:
	void PresentOneFrame();
	void InitializeRenderingResources();
	void SetCommonRenderState( ICommandContext& CmdContext );
	void ReloadRenderPipelines();

	void RenderWorld( ICommandContext& CmdContext, IColorBuffer& RenderTarget );

	IConstantBuffer* GetSceneConstBufferForCurrentFrame();
	IConstantBuffer* GetLightConstBufferForCurrentFrame();

	// Event Processing
	//
	void OnEvent( Event& e );
	bool OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e );

protected:
	Window m_Window;
	HWorld* m_WorldInView;

	InputDispatcher m_InputDispatcher;

	// Rendering Resources
	//
	DeferredShadingTech* m_DeferredShader;
	SkyboxPass* m_SkyPass;
	PostProcesssUber* m_PostProcessPass;
	StaticMeshGeometryRef m_pScreenQuadRef;
	// Constant buffers
	std::vector<IConstantBuffer*> m_pSceneConstantBuffers;
	std::vector<IConstantBuffer*> m_pLightConstantBuffers;
	// Window
	ViewPort	m_ViewPort;
	Rect		m_ScissorRect;

	IColorBuffer* m_pSceneRenderTarget;

};


//
// Inline function implementation
//

inline bool ViewportContext::IsValid()
{
	return GetWindow().IsValid();
}

inline HWorld& ViewportContext::GetWorld()
{
	return *m_WorldInView;
}

inline void ViewportContext::SetWorld( HWorld* pWorldToView )
{
	m_WorldInView = pWorldToView;
}

inline InputDispatcher* ViewportContext::GetInputDispatcher()
{
	return &m_InputDispatcher;
}

inline void ViewportContext::LockMouseToScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().AcquireMouse();
}

inline void ViewportContext::UnlockMouseFromScreenCenter()
{
	m_InputDispatcher.GetInputSureyor().UnacquireMouse();
}

inline void ViewportContext::PresentOneFrame()
{
	m_Window.PresentOneFrame();
}

inline FVector2 ViewportContext::GetDimensions() 
{
	return GetWindow().GetDimensions();
}

inline void ViewportContext::BringToFocus()
{
	GetWindow().SetFocus();
}

inline bool ViewportContext::HasFocus()
{
	return m_Window.HasFocus();
}

inline void ViewportContext::Show()
{
	m_Window.Show();
}

inline void ViewportContext::Hide()
{
	m_Window.Hide();
}

inline Window& ViewportContext::GetWindow()
{
	return m_Window;
}

inline IColorBuffer* ViewportContext::GetPreDisplayBuffer()
{
	return m_pSceneRenderTarget;
}

inline IConstantBuffer* ViewportContext::GetSceneConstBufferForCurrentFrame()
{
	return m_pSceneConstantBuffers[m_Window.GetSwapChain()->GetCurrentFrameIndex()];
}

inline IConstantBuffer* ViewportContext::GetLightConstBufferForCurrentFrame()
{
	return m_pLightConstantBuffers[m_Window.GetSwapChain()->GetCurrentFrameIndex()];
}

inline ViewPort& ViewportContext::GetClientViewport()
{ 
	return m_ViewPort; 
}

inline Rect& ViewportContext::GetClientRect()
{
	return m_ScissorRect; 
}

inline bool ViewportContext::IsPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsPressed( Key );
}

inline bool ViewportContext::IsFirstPressed( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsFirstPressed( Key );
}

inline bool ViewportContext::IsReleased( DigitalInput Key )
{
	return GetInputDispatcher()->GetInputSureyor().IsReleased( Key );
}

inline float ViewportContext::GetMouseMoveDeltaX()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaX();
}

inline float ViewportContext::GetMouseMoveDeltaY()
{
	return GetInputDispatcher()->GetInputSureyor().GetMouseMoveDeltaY();
}
