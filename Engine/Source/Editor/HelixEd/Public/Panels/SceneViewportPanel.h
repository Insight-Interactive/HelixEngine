// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Panels/Panel.h"

#include "Engine/Renderer/DescriptorHeap.h"


class ADebugActor;
class EngineBeginPlayEvent;
class EngineEndPlayEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;


class SceneViewportPanel : public Panel
{
	using Super = Panel;
	friend class HelixEdHomeUI;
public:
	SceneViewportPanel();
	virtual ~SceneViewportPanel();

	virtual void Initialize( FViewportContext* pOwningViewport ) override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

	/*
		Set the debug camera as the main rendering camera 
		and activate its controls
	*/
	void ActivateDebugCamera();
	
	/*
		Deactivate the debug camera's controls.
	*/
	void DeactivateDebugCamera();

	ADebugActor* GetDebugPawn();

protected:
	void FreezeDebugCamera();
	void UnFreezeDebugCamera();

	virtual void OnEvent( Event& e ) override;

	bool OnAppBeginPlay( EngineBeginPlayEvent& e );
	bool OnAppEndPlay( EngineEndPlayEvent& e );
	bool OnMouseButtonPressed( MouseButtonPressedEvent& e );
	bool OnMouseButtonReleased( MouseButtonReleasedEvent& e );


private:
	FDescriptorHandle m_DescriptorHandle;

	ADebugActor* m_pDebugPawn;

	bool m_IsCameraRotating;
};

//
// Inline function implementations
//

inline ADebugActor* SceneViewportPanel::GetDebugPawn()
{
	return m_pDebugPawn;
}
