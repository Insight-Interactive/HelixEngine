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

	void Initialize( const Window::Description& WindowDesc );
	void Uninitialize();
	void Update( float DeltaTime );
	void Render();

	Window& GetWindow();
	IColorBuffer* GetPreDisplayBuffer();
	InputDispatcher* GetInputDispatcher();
	
	void LockMouseToScreenCenter();
	void UnlockMouseFromScreenCenter();

	void BringToFocus();
	bool HasFocus();
	void Show();
	void Hide();

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
