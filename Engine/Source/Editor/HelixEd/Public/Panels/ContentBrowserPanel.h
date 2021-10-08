#pragma once


#include "Panels/Panel.h"


class ContentBrowserPanel : public Panel
{
public:
	ContentBrowserPanel();
	virtual ~ContentBrowserPanel();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

protected:
	void TraverseFolder( const TChar* Folder );

};
