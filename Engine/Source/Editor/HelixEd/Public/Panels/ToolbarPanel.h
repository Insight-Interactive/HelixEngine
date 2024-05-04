// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Panels/Panel.h"

class ToolbarPanel : public Panel
{
public:
	ToolbarPanel();
	virtual ~ToolbarPanel();
	
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;
};
