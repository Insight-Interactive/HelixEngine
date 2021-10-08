#pragma once

#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"


class FCommandContext;
class FViewportContext;

class Panel : public EventEmitter<void, Event&>
{
	friend class HEditorEngine;
	friend class TabbedEditorViewport;
public:
	virtual void Initialize() {}
	virtual void UnInitialize() {}
	
	virtual void Tick( float DeltaTime ) {}
	virtual void Render( FCommandContext& CmdCtx ) {}

	FViewportContext* GetOwningViewport();

protected:
	Panel() 
	{
	}
	virtual ~Panel()
	{
	}

	void SetOwningViewport( FViewportContext* pOwningViewport );

protected:
	FViewportContext* m_OwningViewport;

};

//
// Inline function implementations
//

inline FViewportContext* Panel::GetOwningViewport()
{
	return m_OwningViewport;
}

inline void Panel::SetOwningViewport( FViewportContext* pOwningViewport )
{
	m_OwningViewport = pOwningViewport;
}
