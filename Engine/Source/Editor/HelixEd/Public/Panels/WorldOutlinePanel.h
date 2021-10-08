#pragma once

#include "Panels/Panel.h"

class WorldOutlinePanel : public Panel
{
public:
	WorldOutlinePanel();
	virtual ~WorldOutlinePanel();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;
};
