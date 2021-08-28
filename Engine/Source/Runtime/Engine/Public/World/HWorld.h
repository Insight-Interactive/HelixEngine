// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "World/CameraManager.h"


class HLevel;
class HCameraComponent;
class ICommandContext;
class ViewportContext;

class HWorld
{
public:
	HWorld();
	~HWorld();

	void Initialize();
	void Flush();
	float GetDeltaTime() const;

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(ICommandContext& CmdContext);

	CameraManager* GetCameraManager();
	ViewportContext* GetOwningViewport();
	HCameraComponent* GetCurrentSceneRenderCamera();
	void SetCurrentSceneRenderCamera(HCameraComponent* pCamera);
	void SetViewport( ViewportContext* pViewport );

protected:
	TDynamicArray<HLevel> m_Levels;
	HCameraComponent* m_RenderingCamera;
	ViewportContext* m_pViewport;

	CameraManager m_CameraManager;
};


//
// Inline function implementations
//

inline CameraManager* HWorld::GetCameraManager()
{
	return &m_CameraManager;
}

inline HCameraComponent* HWorld::GetCurrentSceneRenderCamera()
{
	return m_RenderingCamera;
}

inline void HWorld::SetCurrentSceneRenderCamera(HCameraComponent* pCamera)
{
	m_RenderingCamera = pCamera;
}

inline ViewportContext* HWorld::GetOwningViewport()
{
	return m_pViewport;
}

inline void HWorld::SetViewport( ViewportContext* pViewport )
{
	m_pViewport = pViewport;
}