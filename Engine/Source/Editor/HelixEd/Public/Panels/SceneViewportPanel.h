#pragma once

#include "Panels/Panel.h"

#include "DescriptorHeap.h"


class ADebugPawn;

class SceneViewportPanel : public Panel
{
public:
	SceneViewportPanel();
	virtual ~SceneViewportPanel();

	virtual void Initialize() override;
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

	ADebugPawn* GetDebugPawn();

protected:
	void FreezeDebugCamera();
	void UnFreezeDebugCamera();

private:
	FDescriptorHandle m_DescriptorHandle;
	uint32 m_HandleSize;

	ADebugPawn* m_pDebugPawn;

	bool m_IsCameraRotating;
};

//
// Inline function implementations
//

inline ADebugPawn* SceneViewportPanel::GetDebugPawn()
{
	return m_pDebugPawn;
}
