#include "HelixEdPCH.h"

#include "Panels/Panel.h"

#include "Editor/Tabs/ContentEditorInterface.h"


FViewportContext& Panel::GetOwningViewport()
{
	return *m_pOwner;
}
