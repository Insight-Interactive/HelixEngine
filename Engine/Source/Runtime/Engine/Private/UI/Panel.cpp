#include "EnginePCH.h"

#include "UI/Panel.h"

#include "CommandContext.h"
#include "UI/Widget.h"


FUIPanel::FUIPanel()
{
}

FUIPanel::~FUIPanel()
{
}

void FUIPanel::AddWidget( FUIWidget& Widget )
{ 
	m_Widgets.push_back( &Widget );
	Widget.Initialize();
}

bool FUIPanel::RemoveWidget( FUIWidget& Widget )
{
	auto Iter = std::find( m_Widgets.begin(), m_Widgets.end(), &Widget );
	if (Iter != m_Widgets.end())
	{
		m_Widgets.erase( Iter );
		return true;
	}
	return false;
}

void FUIPanel::RenderWidgets( FCommandContext& UIContext )
{
	for (FUIWidget* pWidget : m_Widgets)
	{
		pWidget->Render( UIContext );
	}
}
