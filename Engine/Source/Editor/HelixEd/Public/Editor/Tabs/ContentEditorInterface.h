// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/ViewportContext.h"

#include "UI/EditorUI.h"


const uint32 kMaxTabNameLength = 128;

class Event;
class FViewportContext;
class FCommandContext;

class ContentEditorInterface : public FViewportContext
{
	using Super = FViewportContext;
public:
	virtual void Tick( float DeltaTime );
	virtual void Render( FCommandContext& CmdContext );
	virtual void OnEvent( Event& e );

	// Activate the tab and setup any dependent subpanels this tab posesses.
	void Activate();
	// Deactivate the tab and destroy any dependent panels this tab posesses.
	void Deactivate();

protected:
	ContentEditorInterface( const TChar* Title );
	virtual ~ContentEditorInterface();

	virtual void SetupPanels();
	virtual void TeardownPanels();

	virtual void OnActivated() {}
	virtual void OnDeactivated() {}

	void CreateUIContext();
	void BeginNewFrame();
	void EndFrame( FCommandContext& CmdContext );

private:
	// True if the tab is currently open and resides on the tab bar (or in another user defined location), false if not.
	bool m_IsOpen;
	EditorUIContext m_UIContext;

};


// 
// Inline function implementations
//

inline void ContentEditorInterface::BeginNewFrame()
{
	m_UIContext.BeginNewFrame();
}

inline void ContentEditorInterface::EndFrame( FCommandContext& CmdContext )
{
	m_UIContext.EndFrame( CmdContext );
}
