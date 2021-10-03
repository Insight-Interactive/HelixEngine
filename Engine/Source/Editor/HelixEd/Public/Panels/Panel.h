#pragma once

#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"


class ICommandContext;
class ViewportContext;

class Panel : public EventEmitter<void, Event&>
{
	friend class HEditorEngine;
	friend class TabbedEditorViewport;
public:
	virtual void Initialize() {}
	virtual void UnInitialize() {}
	
	virtual void Tick( float DeltaTime ) {}
	virtual void Render( ICommandContext& CmdCtx ) {}

	ViewportContext* GetOwningViewport();

protected:
	Panel() 
	{
	}
	virtual ~Panel()
	{
	}

	void SetOwningViewport( ViewportContext* pOwningViewport );

protected:
	ViewportContext* m_OwningViewport;

};

//
// Inline function implementations
//

inline ViewportContext* Panel::GetOwningViewport()
{
	return m_OwningViewport;
}

inline void Panel::SetOwningViewport( ViewportContext* pOwningViewport )
{
	m_OwningViewport = pOwningViewport;
}
