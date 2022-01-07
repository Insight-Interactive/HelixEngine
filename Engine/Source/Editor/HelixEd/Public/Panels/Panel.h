#pragma once

#include "Engine/Event/Event.h"
#include "Engine/Event/EventEmitter.h"


class FCommandContext;
class FViewportContext;

class Panel : public EventEmitter<void, Event&>
{
	friend class HEditorEngine;
	friend class ContentEditorInterface;
	friend class HelixEdHomeUI;
public:
	virtual void Initialize( FViewportContext* pOwningViewport );
	virtual void UnInitialize() {}
	
	virtual void Tick( float DeltaTime ) {}
	virtual void Render( FCommandContext& CmdCtx ) {}

	virtual void OnEvent( Event& e ) {}

	bool GetIsActive() const;
	bool SetIsActive( bool IsActive );
	FViewportContext& GetOwningViewport();

protected:
	Panel() 
		: m_IsActive( true )
	{
	}
	virtual ~Panel()
	{
	}

	void SetOwner( FViewportContext* pOwningTab );

protected:
	FViewportContext* m_pOwner;
	bool m_IsActive;

};

//
// Inline function implementations
//

inline void Panel::Initialize( FViewportContext* pOwningViewport )
{ 
	SetOwner( pOwningViewport );
}

inline bool Panel::GetIsActive() const
{
	return m_IsActive;
}

inline bool Panel::SetIsActive( bool IsActive )
{
	m_IsActive = IsActive;
}

inline void Panel::SetOwner( FViewportContext* pOwningTab )
{
	m_pOwner = pOwningTab;
}
