#pragma once

#include "Panels/Panel.h"

#include "Color.h"


class ConsoleOutputPanel : public Panel
{
public:
	ConsoleOutputPanel();
	virtual ~ConsoleOutputPanel();

	virtual void UnInitialize();

	virtual void Tick( float DeltaTime );
	virtual void Render( FCommandContext& CmdCtx );

protected:

};
