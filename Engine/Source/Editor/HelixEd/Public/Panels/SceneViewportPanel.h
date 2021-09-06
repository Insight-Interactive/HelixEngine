#pragma once

#include "Panels/Panel.h"

#include "IDescriptorHeap.h"

class ADebugCharacter;

class SceneViewportPanel : public Panel
{
public:
	SceneViewportPanel();
	virtual ~SceneViewportPanel();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( ICommandContext& CmdCtx ) override;

private:
	DescriptorHandle m_DescriptorHandle;
	uint32 m_HandleSize;

	ADebugCharacter* m_pDebugPawn;

	float m_DeltaTime;
	bool m_IsCameraRotating;
};
