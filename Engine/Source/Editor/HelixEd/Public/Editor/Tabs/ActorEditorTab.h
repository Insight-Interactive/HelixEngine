// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Editor/Tabs/ContentEditorInterface.h"

#include "World/World.h"
#include "Panels/SceneViewportPanel.h"


class ADebugActor;

class ActorEditorTab : public ContentEditorInterface
{
	using Super = ContentEditorInterface;
public:
	ActorEditorTab( const TChar* ActorName, HEditorEngine& EditorEngine );
	virtual ~ActorEditorTab();
	
	virtual void Render( FCommandContext& CmdContext ) override;
	virtual void Tick( float DeltaTime ) override;

private:
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;
	virtual void SetupPanels() override;

private:
	// The world the actor will be edited in.
	HWorld m_ActorWorld;

	// Panels
	SceneViewportPanel m_SceneViewport;

};
