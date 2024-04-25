#pragma once

#include "Editor/Tabs/ContentEditorInterface.h"


class HEditorEngine;

class PreferencesTab : public ContentEditorInterface
{
	using Super = ContentEditorInterface;
public:
	PreferencesTab();
	virtual ~PreferencesTab();

	virtual void Render( FCommandContext& CmdContext ) override;


private:
	void SetupPanels() override;

protected:


};
