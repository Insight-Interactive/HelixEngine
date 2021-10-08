#pragma once

#include "Panels/Panel.h"

class ToolbarPanel : public Panel
{
public:
	ToolbarPanel();
	virtual ~ToolbarPanel();
	
	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;
};
