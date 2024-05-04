// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


#include "Panels/Panel.h"


class ContentBrowserPanel : public Panel
{
	using Super = Panel;
public:
	ContentBrowserPanel();
	virtual ~ContentBrowserPanel();

	virtual void Initialize( FViewportContext* pOwningTab ) override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

protected:
	void TraverseFolder( const TChar* Folder );
	HName m_ProjectContentRoot;
};
