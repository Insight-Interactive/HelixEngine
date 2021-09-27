#pragma once

#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"

class ICommandContext;

class Panel : public EventEmitter<void, Event&>
{
public:
	virtual void Initialize() {}
	virtual void UnInitialize() {}
	
	virtual void Tick( float DeltaTime ) {}
	virtual void Render( ICommandContext& CmdCtx ) {}

protected:
	Panel() 
	{
	}
	virtual ~Panel()
	{
	}

};
