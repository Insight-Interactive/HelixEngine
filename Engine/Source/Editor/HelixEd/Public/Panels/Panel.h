#pragma once


class ICommandContext;

class Panel
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
